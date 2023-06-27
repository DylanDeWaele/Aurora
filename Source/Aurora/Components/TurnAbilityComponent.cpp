// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Aurora/Actors/TurnBomb.h"
#include "Components/ArrowComponent.h"

UTurnAbilityComponent::UTurnAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Set the max ammo to 1, we only spawn 1 bomb
	MaxAmmo = 3;
}

void UTurnAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnSocket = GetSpawnSocket();
}

void UTurnAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If the bomb is not thrown yet then we need it to follow the staff
	if (SpawnedBomb && SpawnedBomb->IsThrown())
	{
		SpawnedBomb = nullptr;
	}
}

void UTurnAbilityComponent::ActivateAbility()
{
	if (TurnBomb && !SpawnedBomb && SpawnSocket)
	{
		//Spawn the turn bomb at the location of the spawn location
		SpawnedBomb = GetWorld()->SpawnActor<ATurnBomb>(TurnBomb, SpawnSocket->GetComponentTransform());
		SpawnedBomb->AttachToComponent(SpawnSocket, FAttachmentTransformRules::KeepWorldTransform);
	}
}

USceneComponent* UTurnAbilityComponent::GetSpawnSocket() const
{
	TArray<AActor*> turnbombSpawns{};
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "TurnBombSpawn", turnbombSpawns);

	for (AActor* spawn : turnbombSpawns)
	{
		//Get the socket component
		TArray<UActorComponent*> sockets = spawn->GetComponentsByTag(USceneComponent::StaticClass(), "Socket");

		for (UActorComponent* socket : sockets)
			return Cast<USceneComponent>(socket);
	}

	return nullptr;
}
