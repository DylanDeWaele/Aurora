// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyShootProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Aurora/Characters/BaseVRCharacter.h"
#include "Aurora/Components/HealthComponent.h"
#include "Aurora/Actors/KillProjectile.h"

// Sets default values
AEnemyShootProjectile::AEnemyShootProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void AEnemyShootProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemyShootProjectile::OnBeginOverlap);

}

void AEnemyShootProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto character = Cast<ABaseVRCharacter>(OtherActor);

	if (IsValid(character))
	{
		auto playerHealth = Cast<UHealthComponent>(character->GetComponentByClass(UHealthComponent::StaticClass()));

		if (IsValid(playerHealth))
		{
			playerHealth->DecreaseHealth(DamageAmount);
		}

		Destruction();
	}
	else
	{
		auto playerProjectile = Cast<AKillProjectile>(OtherActor);
		if (playerProjectile)
		{
			Destroy();
		}
	}
}

void AEnemyShootProjectile::Destruction()
{
	Destroy();
}

// Called every frame
void AEnemyShootProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

