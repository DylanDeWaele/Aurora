// Fill out your copyright notice in the Description page of Project Settings.


#include "StarField.h"
#include "Star.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/AudioComponent.h"

#include "../Enums/Ability.h"
#include "../Actors/AudioManager.h"
#include "../GameModes/AuroraGameModeBase.h"

#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "NiagaraComponent.h"

#define STARFIELD ECC_GameTraceChannel2

int32 AStarField::AmountOfCreatedStars;

AStarField::AStarField()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	SetRootComponent(BoxCollider);

	DrawnSpline = CreateDefaultSubobject<USplineComponent>(TEXT("DrawnSpline"));
	DrawnSpline->SetupAttachment(GetRootComponent());
	DrawnSpline->ClearSplinePoints(true);

	PreviewSpline = CreateDefaultSubobject<USplineComponent>(TEXT("CorrectSpline"));
	PreviewSpline->SetupAttachment(GetRootComponent());
	PreviewSpline->ClearSplinePoints(true);

	SetupStars();
}

//-------------------
//SETUP
//-------------------
void AStarField::SetupStars()
{
	for (size_t i = 0; i < AmountOfStars; ++i)
	{
		UChildActorComponent* star = CreateDefaultSubobject<UChildActorComponent>(FName{ FString::Printf(TEXT("Star %i"), i) });
		star->CreateChildActor();
		star->SetupAttachment(GetRootComponent());
		Stars.Add(star);
	}
}

void AStarField::SetCollisionOnPreviewStars(bool CollsionOn)
{
	if (PreviewPattern.Num() == 0)
		return;

	for (int32 index : PreviewPattern)
	{
		auto star = Cast<AStar>(Stars[index]->GetChildActor());

		if (star)
		{
			star->SetActorEnableCollision(CollsionOn);
		}
	}
}

void AStarField::HideAllStars()
{
	for (UChildActorComponent* component : Stars)
	{
		component->SetVisibility(false);
	}
}

void AStarField::SetIndices()
{
	AmountOfCreatedStars = 0;

	for (UChildActorComponent* star : Stars)
	{
		AStar* starCast = Cast<AStar>(star->GetChildActor());
		if (starCast)
		{
			starCast->SetStarIndex(AmountOfCreatedStars++);
		}
	}
}

void AStarField::SetupCollision()
{
	if (BoxCollider)
	{
		BoxCollider->SetCollisionObjectType(STARFIELD);
		BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		BoxCollider->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	}

	for (UChildActorComponent* component : Stars)
	{
		AStar* star = Cast<AStar>(component->GetChildActor());

		if (star)
		{
			star->SetActorEnableCollision(false);
		}
	}
}

const TArray<int32>& AStarField::GetDrawnPattern() const
{
	return DrawnPattern;
}

void AStarField::BeginPlay()
{
	Super::BeginPlay();

	SetIndices();
	SetupCollision();

	CreateRandomPattern();

	HideAllStars();
	ShowPreview(false);

	AudioManager = GetAudioManager();
}


// Called every frame
void AStarField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentHoverSound && CurrentHoverSound->IsPendingKill())
		CurrentHoverSound = nullptr;
}

//-------------------
//STARS
//-------------------

void AStarField::NewStarHit(int32 StarIndex)
{
	if (AudioManager)
	{
		switch (Ability)
		{
		case EAbility::Kill:
			AudioManager->PlaySound(EType::SFX, "Stars_Connect_1");
			break;
		case EAbility::Purify:
			AudioManager->PlaySound(EType::SFX, "Stars_Connect_2");
			break;
		case EAbility::Turn:
			AudioManager->PlaySound(EType::SFX, "Stars_Connect_3");
			break;
		}
	}

	DrawnPattern.Add(StarIndex);

	AStar* newStarHit = Cast<AStar>(Stars[StarIndex]->GetChildActor());
	if (newStarHit)
	{
		newStarHit->Activate();
	}

	if (DrawnSpline)
	{
		//calculate an offset location so it doesn't overlap the preview spline
		FVector pointLocation = Stars[StarIndex]->GetComponentLocation() + Stars[StarIndex]->GetForwardVector() * DrawOffset;

		//add spline point
		DrawnSpline->AddSplinePoint(pointLocation, ESplineCoordinateSpace::World);
		DrawnSpline->SetSplinePointType(DrawnSpline->GetNumberOfSplinePoints() - 1, ESplinePointType::Linear);

		if (DrawnPattern.Num() != 1)
		{
			ConstructSplineMesh(DrawnSpline->GetNumberOfSplinePoints() - 1, DrawnMesh, DrawnSpline, DrawnSplineMeshes, false);
		}
	}
}

void AStarField::DeActivateStars()
{
	if (!DrawnPattern.Num())
		return;

	for (int32 index : DrawnPattern)
	{
		AStar* currentStar = Cast<AStar>(Stars[index]->GetChildActor());
		if (currentStar)
		{
			currentStar->DeActivate();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("wrong type conversion in DeactiveStars"));
			return;
		}
	}

	DrawnPattern.Empty();

	DestructSplineMesh(DrawnSpline, DrawnSplineMeshes);
}

//-------------------
//SPLINES
//-------------------

void AStarField::ShowPreview(bool Enable)
{
	for (int32 index : PreviewPattern)
	{
		Stars[index]->SetVisibility(Enable);
	}

	TogglePreviewSpline(Enable);
	SetCollisionOnPreviewStars(Enable);

	//Play the hover sound
	if (AudioManager && Enable)
		CurrentHoverSound = AudioManager->PlaySound(EType::SFX, "Stars_Hover");
}

void AStarField::TogglePreviewSpline(bool Enable)
{
	if (PreviewSpline)
	{
		for (auto splineMesh : PreviewSplineMeshes)
		{
			splineMesh->SetVisibility(Enable, true);
		}
	}
}

void AStarField::ConstructSplineMesh(int32 Index, UStaticMesh* const SplineMesh, USplineComponent* const Spline, TArray<USplineMeshComponent*>& SplineMeshList, bool IsPreviewSpline)
{
	if (!SplineMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("spline mesh is not set in editor"));
		return;
	}

	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());

	//SplineMeshComponent->SetStaticMesh(SplineMesh);
	SplineMeshComponent->SetMobility(EComponentMobility::Movable);
	SplineMeshComponent->CreationMethod = EComponentCreationMethod::Instance;
	SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
	SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);

	const FVector StartPos = Spline->GetLocationAtSplinePoint(Index - 1, ESplineCoordinateSpace::Local);
	FVector StartTangent = Spline->GetTangentAtSplinePoint(Index - 1, ESplineCoordinateSpace::Local);
	const FVector EndPos = Spline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::Local);
	FVector EndTangent = Spline->GetTangentAtSplinePoint(Index, ESplineCoordinateSpace::Local);

	//removes undefined behavior of rendering of spline meshes
	if (StartTangent.X == 0)
	{
		StartTangent.X = 0.00001f;
	}
	if (EndTangent.X == 0)
	{
		EndTangent.X = 0.00001f;
	}
	if (StartTangent.Y == 0)
	{
		StartTangent.Y = 0.00001f;
	}
	if (EndTangent.Y == 0)
	{
		EndTangent.Y = 0.00001f;
	}

	SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);

	SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);

	//create the niagra effect
	if (IsValid(DrawnSplineMeshParticles) && IsValid(SplineMeshComponent))
	{
		UNiagaraComponent* NiagraComponent = NewObject<UNiagaraComponent>(this, UNiagaraComponent::StaticClass());
		if(!IsPreviewSpline)
			NiagraComponent->SetAsset(DrawnSplineMeshParticles);
		else
			NiagraComponent->SetAsset(PreviewSplineMeshParticles);

		NiagraComponent->CreationMethod = EComponentCreationMethod::Instance;
		NiagraComponent->SetMobility(EComponentMobility::Movable);
		NiagraComponent->RegisterComponentWithWorld(GetWorld());
		NiagraComponent->AttachToComponent(SplineMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
		NiagraComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		NiagraComponent->SetRelativeLocation(StartPos);
		FVector splineVector = EndPos - StartPos;
		FRotator splineRotator = splineVector.Rotation();

		NiagraComponent->SetRelativeRotation(splineRotator);
		float length = 0.f;
		if(IsPreviewSpline)
			length = splineVector.Size() / PreviewSplineDivider;
		else
			length = splineVector.Size() / DrawSplineDivider;
		NiagraComponent->SetVariableFloat(TEXT("LifeTime"), length);


	}

	SplineMeshList.Add(SplineMeshComponent);
}

void AStarField::DestructSplineMesh(USplineComponent* const Spline, TArray<USplineMeshComponent*>& SplineMeshes)
{
	//clear splines + meshes
	Spline->ClearSplinePoints();

	if (!SplineMeshes.Num())
		return;

	for (auto splineMeshComponent : SplineMeshes)
	{
		if (splineMeshComponent)
		{
			auto particle = Cast<UNiagaraComponent>(splineMeshComponent->GetChildComponent(0));
			if (IsValid(particle))
			{
				particle->DestroyComponent();
			}
			splineMeshComponent->DestroyComponent(false);
		}
	}
	SplineMeshes.Empty();
}

void AStarField::MakeNewPreviewSpline()
{
	if (!PreviewMesh || PreviewPattern.Num() == 0)
		return;

	//add spline point
	for (int32 index : PreviewPattern)
	{
		PreviewSpline->AddSplinePoint(Stars[index]->GetComponentLocation(), ESplineCoordinateSpace::World);
		PreviewSpline->SetSplinePointType(PreviewSpline->GetNumberOfSplinePoints() - 1, ESplinePointType::Linear);
		ConstructSplineMesh(PreviewSpline->GetNumberOfSplinePoints() - 1, PreviewMesh, PreviewSpline, PreviewSplineMeshes, true);
	}

}

void AStarField::SetBeginPointOfPreviewSpline()
{
	if (PreviewPattern.Num() == 0)
		return;

	auto beginStar = Cast<AStar>(Stars[PreviewPattern[0]]->GetChildActor());

	if (beginStar)
	{
		beginStar->MarkAsStart();
	}
}

void AStarField::ResetBeginPointOfPreviewSpline()
{
	if (PreviewPattern.Num() == 0)
		return;

	auto beginStar = Cast<AStar>(Stars[PreviewPattern[0]]->GetChildActor());

	if (beginStar)
	{
		beginStar->UnMarkAsStart();
	}
}

//-------------------
//PATTERNS
//-------------------
void AStarField::ClearDrawnPattern()
{
	if (DrawnPattern.Num() != 0)
	{
		DrawnPattern.Empty();
	}
}

void AStarField::CreateRandomPattern()
{
	ResetPreviousPattern();

	MakeNewPreviewPattern();
}


void AStarField::ResetPreviousPattern()
{
	if (PreviewPattern.Num() == 0)
		return;

	ResetBeginPointOfPreviewSpline();
	ShowPreview(false);

	PreviewPattern.Empty();

	DestructSplineMesh(PreviewSpline, PreviewSplineMeshes);
}

void AStarField::MakeNewPreviewPattern()
{
	if (PreviewPattern.Num() != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("there is still a preview pattern loaded"));
		return;
	}

	int32 numberOfStarsNewPattern = FMath::RandRange(MinimumPatternSize, MaximumPatternSize);

	for (size_t i{}; i < numberOfStarsNewPattern; ++i)
	{
		int32 newStarIndex = FMath::RandRange(0, AmountOfCreatedStars - 1);
		if (!PreviewPattern.Contains<int32>(newStarIndex))
		{
			PreviewPattern.Add(newStarIndex);
		}
		else
		{
			--i;
			//if really unlucky this for loop can run for a while.
		}
	}

	SetBeginPointOfPreviewSpline();
	MakeNewPreviewSpline();
	ShowPreview(false);
}

bool AStarField::IsPatternCorrect() const
{
	if (DrawnPattern.Num() == 0)
		return false;

	if (DrawnPattern.Num() != PreviewPattern.Num())
	{
		return false;
	}

	for (int32 i = 0; i < DrawnPattern.Num(); ++i)
	{
		if (DrawnPattern[i] != PreviewPattern[i])
		{
			return false;
		}
	}

	return true;
}

void AStarField::SetNewPatternOnCompleted()
{
	if (IsPatternCompleted)
	{
		IsPatternCompleted = false;
	}
}

void AStarField::OnCorrectPattern()
{
	//Spawn completion particles on the last star
	if (StarCompletionParticles && DrawnPattern.Num() > 0)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), StarCompletionParticles, Stars[DrawnPattern.Last(0)]->GetComponentLocation(), FRotator::ZeroRotator);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);

	//create new pattern
	//but don't show it
	IsPatternCompleted = true;
	DeActivateStars();
	CreateRandomPattern();
}

AAudioManager* AStarField::GetAudioManager() const
{
	//Get audio manager
	AAuroraGameModeBase* auroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (auroraGameMode)
		return auroraGameMode->GetAudioManager();

	UE_LOG(LogTemp, Warning, TEXT("Current gamemode is not the Aurora game mode."));
	return nullptr;
}