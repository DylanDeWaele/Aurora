// Fill out your copyright notice in the Description page of Project Settings.


#include "Staff.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Aurora/Actors/CrosshairManager.h"
#include "Aurora/Actors/StaffCrystals.h"
#include "Aurora/Actors/AudioManager.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AStaff::AStaff()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create meshes
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	MeshComponent->ComponentTags.Add("GrabMesh");

	Crystals = CreateDefaultSubobject<UChildActorComponent>(TEXT("Crystals"));
	Crystals->SetupAttachment(MeshComponent);

	DecorationOne = CreateDefaultSubobject<UStaticMeshComponent>("DecorationOne");
	DecorationOne->SetupAttachment(MeshComponent);

	DecorationTwo = CreateDefaultSubobject<UStaticMeshComponent>("DecorationTwo");
	DecorationTwo->SetupAttachment(MeshComponent);

	//Create particle systems
	NeutralParticleSystemComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NeutralParticles"));
	NeutralParticleSystemComponent->SetupAttachment(MeshComponent);
	NeutralParticleSystemComponent->bAutoActivate = false;

	KillAbilityParticleSystemComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("KillAbilityParticles"));
	KillAbilityParticleSystemComponent->SetupAttachment(MeshComponent);
	KillAbilityParticleSystemComponent->bAutoActivate = false;

	PurifyAbilityParticleSystemComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PurifyAbilityParticles"));
	PurifyAbilityParticleSystemComponent->SetupAttachment(MeshComponent);
	PurifyAbilityParticleSystemComponent->bAutoActivate = false;

	//Create sockets
	FrontSocketComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontSocket"));
	BackSocketComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackSocket"));
	FrontSocketComponent->SetupAttachment(MeshComponent);
	BackSocketComponent->SetupAttachment(MeshComponent);

	BackSocket.Component = BackSocketComponent;
	BackSocket.bIsBack = true;
	FrontSocket.Component = FrontSocketComponent;
	FrontSocket.bIsBack = false;
}

// Called when the game starts or when spawned
void AStaff::BeginPlay()
{
	Super::BeginPlay();

	AudioManager = GetAudioManager();

	GetHandMeshes();
}

// Called every frame
void AStaff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FrontSocket.Controller && BackSocket.Controller)
		RotateTowardsFrontSocket();

	HandleSocketDisplay();

	//Handle sounds deactivation
	if (KillHoldSound && KillHoldSound->IsPendingKill())
		KillHoldSound = nullptr;
	if (PurifyHoldSound && PurifyHoldSound->IsPendingKill())
		PurifyHoldSound = nullptr;

	//Handle the sound following the crystals
	if (KillHoldSound)
		KillHoldSound->SetWorldLocation(Crystals->GetComponentLocation());
	else if (PurifyHoldSound)
		PurifyHoldSound->SetWorldLocation(Crystals->GetComponentLocation());
}

void AStaff::SetAbility(EAbility Type)
{
	//Turn off the current particle effect
	if (CurrentParticleSystemComponent)
		CurrentParticleSystemComponent->Deactivate();

	if (KillHoldSound)
		KillHoldSound->Stop();
	if (PurifyHoldSound)
		PurifyHoldSound->Stop();

	switch (Type)
	{
	case EAbility::None:
		CurrentParticleSystemComponent = NeutralParticleSystemComponent;
		break;
	case EAbility::Kill:
		CurrentParticleSystemComponent = KillAbilityParticleSystemComponent;

		//Play the ignition sound when the ability has been activated and then the flame sound after
		AudioManager->PlaySound(EType::SFX, "Kill_Ignition");
		KillHoldSound = AudioManager->PlaySound(EType::SFX, "Kill_Flame", 0, Crystals->GetComponentLocation());

		break;
	case EAbility::Purify:
		CurrentParticleSystemComponent = PurifyAbilityParticleSystemComponent;

		AudioManager->PlaySound(EType::SFX, "Purify_Ignition");
		PurifyHoldSound = AudioManager->PlaySound(EType::SFX, "Purify_Hold", 0, Crystals->GetComponentLocation());
		break;
	}

	//Turn on the current particle effect
	if (CurrentParticleSystemComponent)
		CurrentParticleSystemComponent->Activate();

	//Set the crystal materials
	GetCrystals()->SetAbility(Type);
}

ACrosshairManager* AStaff::GetCrosshair() const
{
	TArray<UActorComponent*> crosshairActors = GetComponentsByTag(UChildActorComponent::StaticClass(), "Crosshair");

	//Check if we found any
	if (crosshairActors.Num() > 0)
	{
		//Check if its has a child actor assigned
		AActor* crosshairChildActor = Cast<UChildActorComponent>(crosshairActors[0])->GetChildActor(); //We assume we can cast it to the child actor since we are getting the components by class
		if (IsValid(crosshairChildActor))
		{
			return Cast<ACrosshairManager>(crosshairChildActor);
		}

		UE_LOG(LogTemp, Warning, TEXT("No child actor set in the crosshair child actor on the staff."));
		return nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("No crosshair actor found on the staff."));
	return nullptr;

}

AStaffCrystals* AStaff::GetCrystals() const
{
	AActor* actor = Crystals->GetChildActor();
	if (actor)
		return Cast<AStaffCrystals>(actor);

	UE_LOG(LogTemp, Warning, TEXT("No crystals child actor set."));
	return nullptr;
}

void AStaff::RotateTowardsFrontSocket()
{
	FRotator rotation = UKismetMathLibrary::MakeRotFromZY(FrontSocket.Controller->GetComponentLocation() - BackSocket.Controller->GetComponentLocation(), GetActorRightVector());

	SetActorRotation(rotation);
}

void AStaff::HandleSocketDisplay()
{
	if (BackSocket.Controller)
	{
		//Set controller invisible
		USceneComponent* mesh = BackSocket.Controller->GetChildComponent(2);
		if (mesh)
			mesh->SetVisibility(false);
		//Set socket invisible
		BackSocket.Component->SetVisibility(false);

		//Set socketed hand visible
		if (BackSocket.bLeftController)
		{
			BackSocketLeftHand->SetVisibility(true);
			BackSocketRightHand->SetVisibility(false);
		}
		else
		{
			BackSocketRightHand->SetVisibility(true);
			BackSocketLeftHand->SetVisibility(false);
		}
	}
	else
	{
		BackSocket.Component->SetVisibility(true);

		if (BackSocket.bLeftController)
			BackSocketLeftHand->SetVisibility(false);
		else
			BackSocketRightHand->SetVisibility(false);
	}

	if (FrontSocket.Controller)
	{
		//Set controller invisible
		USceneComponent* mesh = FrontSocket.Controller->GetChildComponent(2);
		if (mesh)
			mesh->SetVisibility(false);
		//Set socket invisible
		FrontSocket.Component->SetVisibility(false);

		//Set socketed hand visible
		if (FrontSocket.bLeftController)
		{
			FrontSocketLeftHand->SetVisibility(true);
			FrontSocketRightHand->SetVisibility(false);
		}
		else
		{
			FrontSocketRightHand->SetVisibility(true);
			FrontSocketLeftHand->SetVisibility(false);
		}
	}
	else
	{
		FrontSocket.Component->SetVisibility(true);

		//Set socketed hand visible
		if (FrontSocket.bLeftController)
			FrontSocketLeftHand->SetVisibility(false);
		else
			FrontSocketRightHand->SetVisibility(false);
	}
}

AAudioManager* AStaff::GetAudioManager() const
{
	//Get audio manager
	AAuroraGameModeBase* auroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (auroraGameMode)
		return auroraGameMode->GetAudioManager();

	UE_LOG(LogTemp, Warning, TEXT("Current gamemode is not the Aurora game mode."));
	return nullptr;
}

void AStaff::GetHandMeshes()
{
	FrontSocketLeftHand = Cast<UStaticMeshComponent>(GetComponentsByTag(UStaticMeshComponent::StaticClass(), "FrontLeft")[0]);
	FrontSocketRightHand = Cast<UStaticMeshComponent>(GetComponentsByTag(UStaticMeshComponent::StaticClass(), "FrontRight")[0]);

	BackSocketLeftHand = Cast<UStaticMeshComponent>(GetComponentsByTag(UStaticMeshComponent::StaticClass(), "BackLeft")[0]);
	BackSocketRightHand = Cast<UStaticMeshComponent>(GetComponentsByTag(UStaticMeshComponent::StaticClass(), "BackRight")[0]);
}