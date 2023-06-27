// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemySpawner.h"
#include "Aurora/Components/PathNodeNetworkSceneComponent.h"
#include "Aurora/Characters/BaseEnemy.h" 
#include "Aurora/Controllers/BaseEnemyController.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

// Sets default values
ABaseEnemySpawner::ABaseEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	SetRootComponent(BoxCollider);

	PathNodeNetworkComponent = CreateDefaultSubobject<UPathNodeNetworkSceneComponent>(TEXT("PathNodeNetworkComponent"));
	PathNodeNetworkComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABaseEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseEnemySpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	PathNodeNetworkComponent->InitNodeLocations();
}

void ABaseEnemySpawner::SpawnEnemy(TSubclassOf<ABaseEnemy> EnemyToSpawn, EEnemyType TypeOfEnemy)
{
	if(!PathNodeNetworkComponent)
		return;

	//seek x amount of locations out for the enemy
	TArray<FVector> locations{};

	for (size_t i{}; i < PathNodeNetworkComponent->GetNumberOfLocationsPerEnemy(); ++i)
	{
		//start with the furthest one away
		auto location = PathNodeNetworkComponent->GetLocationAtIndex(i * PathNodeNetworkComponent->GetLocationsPerSegment() + (FMath::RandRange(0, PathNodeNetworkComponent->GetLocationsPerSegment() - 1)));
		locations.Add(location);
	}	

	//pick a random location around the spawner
	FVector spawnLocation =	UKismetMathLibrary::RandomPointInBoundingBox(BoxCollider->GetComponentLocation(), BoxCollider->GetScaledBoxExtent());

	//instantiate enemy
	auto actor = GetWorld()->SpawnActor<ABaseEnemy>(EnemyToSpawn, spawnLocation, GetActorForwardVector().Rotation());

	if(!actor)
		return;

	auto enemyController = actor->GetController<ABaseEnemyController>();

	if (enemyController)
	{
		enemyController->InitAndStartMoving(locations, TypeOfEnemy);
	}
}