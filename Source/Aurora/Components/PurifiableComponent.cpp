// Fill out your copyright notice in the Description page of Project Settings.


#include "PurifiableComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Aurora/Actors/PurifyBeam.h"
#include "Aurora/Actors/AudioManager.h"
#include "Aurora/Controllers/BaseEnemyController.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "Aurora/Components/TurnableComponent.h"
#include "Aurora/Components/HealthComponent.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UPurifiableComponent::UPurifiableComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
}

void UPurifiableComponent::BeginPlay()
{
	Super::BeginPlay();

	//Get particles systems on the enemy
	ImpactParticles = GetParticles(TEXT("ImpactParticles"));
	PurifiedParticles = GetParticles(TEXT("PurifiedParticles"));

	if (!ImpactParticles)
		UE_LOG(LogTemp, Warning, TEXT("No niagara particle system component with tag ImpactParticles found on an actor that has a purifiable component."));

	if (!PurifiedParticles)
		UE_LOG(LogTemp, Warning, TEXT("No niagara particle system component with tag PurifiedParticles found on an actor that has a purifiable component."));

	AudioManager = GetAudioManager();

	InitializeDynamicMaterials();
}

void UPurifiableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UTurnableComponent* turnableComponent = Owner->GetTurnableComponent();
	UHealthComponent* healthComponent = Owner->GetHealthComponent();
	bool beingEvaporated = Cast<ABaseEnemy>(Owner)->IsBeingEvaporated();

	if (!beingEvaporated && !bIsPurified && turnableComponent && healthComponent)
	{
		if (!turnableComponent->IsTurned() && !healthComponent->IsDead())
		{
			CheckIfBeingPurified();
			HandlePurification(DeltaTime);
		}
	}
}

bool UPurifiableComponent::IsPurified() const
{
	return bIsPurified;
}

bool UPurifiableComponent::IsBeingPurified() const
{
	return bIsBeingPurified;
}

void UPurifiableComponent::StartPurification()
{
	bIsBeingPurified = true;
}

void UPurifiableComponent::StopPurification()
{
	bIsBeingPurified = false;
}

void UPurifiableComponent::Purify()
{
	SetIsPurifiedInBlackBoard();

	bIsBeingPurified = false;
	bIsPurified = true;

	//Turn on particle effects
	if (ImpactParticles)
		ImpactParticles->Activate();
	if (PurifiedParticles)
		PurifiedParticles->Activate();

	AddToStats(EAbility::Purify, 1);

	//Play sound
	if (AudioManager)
		AudioManager->PlaySound(EType::SFX, "Purify_Impact");
}

UNiagaraComponent* UPurifiableComponent::GetParticles(const FName& Tag) const
{
	TArray<UActorComponent*> particles = GetOwner()->GetComponentsByTag(UNiagaraComponent::StaticClass(), Tag);

	for (UActorComponent* component : particles)
	{
		return Cast<UNiagaraComponent>(component);
	}

	return nullptr;
}

void UPurifiableComponent::CheckIfBeingPurified()
{
	TArray<AActor*> overlappingActors{};

	//get capsule collider because you don't want to check collision with other colliders
	auto collision = Cast<UCapsuleComponent>(GetOwner()->GetComponentByClass(UCapsuleComponent::StaticClass()));
	if (IsValid(collision))
	{
		collision->GetOverlappingActors(overlappingActors);
	}

	for (AActor* actor : overlappingActors)
	{
		APurifyBeam* beam = Cast<APurifyBeam>(actor);

		if (beam)
		{
			if (beam->BeamEnabled())
			{
				bIsBeingPurified = true;
				return;
			}
		}
	}
	bIsBeingPurified = false;
}

void UPurifiableComponent::HandlePurification(float DeltaTime)
{
	if (bIsBeingPurified)
	{
		CurrentPurificationPercentage += PurifcationSpeed * DeltaTime;

		UpdateMaterials();

		if (CurrentPurificationPercentage >= 100)
			Purify();
	}
	else
	{
		//When the actor is not being purified we should decay the current purification percentage
		if (CurrentPurificationPercentage > 0)
		{
			CurrentPurificationPercentage -= DecaySpeed * DeltaTime;
			UpdateMaterials();
		}
	}
}

void UPurifiableComponent::SetIsPurifiedInBlackBoard()
{
	auto blackboard = Cast<AAIController>(Cast<ABaseEnemy>(GetOwner())->GetController())->GetBlackboardComponent();

	if (blackboard)
	{
		blackboard->SetValueAsBool("IsPurified", true);
	}
}

AAudioManager* UPurifiableComponent::GetAudioManager() const
{
	//Get audio manager
	AAuroraGameModeBase* auroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (auroraGameMode)
		return auroraGameMode->GetAudioManager();

	UE_LOG(LogTemp, Warning, TEXT("Current gamemode is not the Aurora game mode."));
	return nullptr;
}

void UPurifiableComponent::InitializeDynamicMaterials()
{
	//Get skeletal mesh
	UActorComponent* actorComponent = GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass());

	if (actorComponent)
	{
		for (int i = 0; i < AmountOfMaterials; i++)
		{
			USkeletalMeshComponent* skeletalMesh = Cast<USkeletalMeshComponent>(actorComponent);

			if (skeletalMesh)
			{
				//Get the material
				UMaterialInterface* material = skeletalMesh->GetMaterial(i);

				if (material)
					DynamicMaterials.Add(skeletalMesh->CreateDynamicMaterialInstance(i, material));
			}
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("No skeletal mesh component found on an actor that has a purifiable component attached to it."));
}

void UPurifiableComponent::UpdateMaterials()
{
	if (DynamicMaterials.Num() > 0)
	{
		for (UMaterialInstanceDynamic* dynamicMaterial : DynamicMaterials)
		{
			dynamicMaterial->SetScalarParameterValue("GoodPercentage", CurrentPurificationPercentage / 100.f);
		}
	}
}