// Fill out your copyright notice in the Description page of Project Settings.


#include "PathNodeNetworkSceneComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPathNodeNetworkSceneComponent::UPathNodeNetworkSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//InitNodeLocations();
	// ...
}


// Called when the game starts
void UPathNodeNetworkSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	SortNodeLocations();
}


// Called every frame
void UPathNodeNetworkSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPathNodeNetworkSceneComponent::InitNodeLocations()
{

	if(!ResetNodes)
		return;

	if(NodeLocations.Num() != 0)
		NodeLocations.Empty();
	
	for (size_t i = 0; i < NumberOfLocationsPerEnemy * LocationsPerSegment; ++i)
	{
		NodeLocations.Add(FVector(0, 0, 0));
	}
}

const FVector UPathNodeNetworkSceneComponent::GetLocationAtIndex(int32 Index) const
{
	if (Index >= NodeLocations.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("trying to access a location thats out of bound: index: %i"), Index);
		return FVector::ZeroVector;
	}

	//transform to absolute location
	return NodeLocations[Index] + GetOwner()->GetActorLocation();
}

//Sort based on the distance between the player and the locations
void UPathNodeNetworkSceneComponent::SortNodeLocations()
{
	auto player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (player)
	{
		auto playerLocation = player->GetActorLocation();
		NodeLocations.Sort([&playerLocation, this](FVector Lhs,FVector Rhs)
			{
			//make them absolute
				Lhs += GetOwner()->GetActorLocation();
				Rhs += GetOwner()->GetActorLocation();
				Lhs.Z = 0;
				Rhs.Z = 0;
				return FVector::Dist(Lhs, playerLocation) > FVector::Dist(Rhs, playerLocation);
			});
	}
}