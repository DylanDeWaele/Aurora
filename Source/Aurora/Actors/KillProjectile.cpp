// Fill out your copyright notice in the Description page of Project Settings.


#include "KillProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Aurora/Components/KillableComponent.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "Aurora/Actors/PurifyBeam.h"
#include "Aurora/Actors/AudioManager.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AKillProjectile::AKillProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create collider
	ColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = ColliderComponent;

	//Create mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(ColliderComponent);

	//Create particles
	ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(MeshComponent);
	ParticleSystem->bAutoActivate = true;

	//Create projectile movement component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AKillProjectile::BeginPlay()
{
	Super::BeginPlay();

	//Link the on hit function with the collision
	ColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &AKillProjectile::OnBeginOverlap);

	//Get audio manager
	AudioManager = GetAudioManager();

	//Play the shoot sound
	if (AudioManager)
		AudioManager->PlaySound(EType::SFX, "Kill_Shoot", 0, GetActorLocation());
}

// Called every frame
void AKillProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKillProjectile::FireInDirection(const FVector& Direction)
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

void AKillProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//this means the comp you overlapped with is a detection box.
	if (OtherComp->ComponentHasTag("DETECTION"))
		return;

	//Check if the actor that we hit has a killable component
	UActorComponent* killableComponent = OtherActor->GetComponentByClass(UKillableComponent::StaticClass());

	//Spawn impact
	UNiagaraComponent* impact = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticleSystem, GetActorLocation(), GetActorRotation(), { 0.75f,0.75f,0.75f });

	impact->DetachFromParent(true);

	if (killableComponent)
	{
		//Deal damage to the actor via the killable component
		Cast<UKillableComponent>(killableComponent)->Damage(DamageAmount);

		//Play enemy explosion sound
		if (AudioManager)
			AudioManager->PlaySound(EType::SFX, "Kill_Explode", 0.f, impact->GetComponentLocation());
	}
	else
	{
		//Play ground explosion sound
		if (AudioManager)
			AudioManager->PlaySound(EType::SFX, "Kill_Explode_Ground", 0.f, impact->GetComponentLocation());
	}

	Destroy();
}

AAudioManager* AKillProjectile::GetAudioManager() const
{
	//Get audio manager
	AAuroraGameModeBase* auroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (auroraGameMode)
		return auroraGameMode->GetAudioManager();

	UE_LOG(LogTemp, Warning, TEXT("Current gamemode is not the Aurora game mode."));
	return nullptr;
}
