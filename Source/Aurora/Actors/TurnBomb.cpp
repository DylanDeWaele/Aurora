// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBomb.h"
#include "Aurora/Components/PickupableComponent.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "Aurora/Actors/AudioManager.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ATurnBomb::ATurnBomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	RootComponent = BombMesh;

	BombMesh->ComponentTags.Add("GrabMesh");

	BombParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BombParticles"));
	BombParticles->SetupAttachment(BombMesh);

	AreaOfEffect = CreateDefaultSubobject<USphereComponent>(TEXT("AOE"));
	AreaOfEffect->SetupAttachment(BombMesh);

	AreaOfEffectParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AOEParticles"));
	AreaOfEffectParticles->SetupAttachment(BombMesh);

	Pickupable = CreateDefaultSubobject<UPickupableComponent>(TEXT("Pickupable"));
}

// Called when the game starts or when spawned
void ATurnBomb::BeginPlay()
{
	Super::BeginPlay();

	BombMesh->OnComponentHit.AddDynamic(this, &ATurnBomb::OnHit);

	AreaOfEffect->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Get audio manager
	AudioManager = GetAudioManager();

	if (AudioManager)
	{
		//Play the spawn sound once
		AudioManager->PlaySound(EType::SFX, "Turn_Spawn",0, GetActorLocation());

		//Play the hold looping sound and attach it to this actor
		HoldSound = AudioManager->PlaySound(EType::SFX, "Turn_Hold", 0, GetActorLocation());
		HoldSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

// Called every frame
void ATurnBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Pickupable->IsGrabbed())
		bIsThrown = true;

	//Play sound when thrown is true but it is not grabbed anymore (actual throw)
	if (bIsThrown && !Pickupable->IsGrabbed() && !bThrowSoundPlayed)
	{
		AudioManager->PlaySound(EType::SFX, "Turn_Throw");
		bThrowSoundPlayed = true;
	}

	if (bExploded)
		ScaleDown(DeltaTime);

	if (HoldSound && HoldSound->IsPendingKill())
		HoldSound = nullptr;

	if (HoldSound)
		HoldSound->SetWorldLocation(GetActorLocation());
}

bool ATurnBomb::IsThrown() const
{
	return bIsThrown;
}

void ATurnBomb::Explode()
{
	if (bIsThrown)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATurnBomb::SetExploded, 1.5f, false, -1.f);

		BombMesh->SetSimulatePhysics(false);
		BombMesh->SetVisibility(false);

		AreaOfEffect->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AreaOfEffectParticles->Activate(true);

		//stop the sound on explode
		if(HoldSound)
			HoldSound->Stop();

		//Play the impact sound
		AudioManager->PlaySound(EType::SFX, "Turn_Impact");

		SetActorRotation(FRotator::ZeroRotator);
		SetLifeSpan(TimeActive);
	}
}

void ATurnBomb::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ATurnBomb::ScaleDown(float DeltaTime)
{
	CurrentScale = AreaOfEffect->GetComponentScale();

	if (CurrentScale.X > 0)
		CurrentScale.X -= ScaleDownSpeed * DeltaTime;
	if (CurrentScale.Y > 0)
		CurrentScale.Y -= ScaleDownSpeed * DeltaTime;
	if (CurrentScale.Z > 0)
		CurrentScale.Z -= ScaleDownSpeed * DeltaTime;

	AreaOfEffect->SetWorldScale3D(CurrentScale);
}

void ATurnBomb::SetExploded()
{
	bExploded = true;
}

AAudioManager* ATurnBomb::GetAudioManager() const
{
	//Get audio manager
	AAuroraGameModeBase* auroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (auroraGameMode)
		return auroraGameMode->GetAudioManager();

	UE_LOG(LogTemp, Warning, TEXT("Current gamemode is not the Aurora game mode."));
	return nullptr;
}