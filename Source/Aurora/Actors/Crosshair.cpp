// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair.h"

// Sets default values
ACrosshair::ACrosshair()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = root;
}

// Called when the game starts or when spawned
void ACrosshair::BeginPlay()
{
	Super::BeginPlay();

	CrosshairDot = GetCrosshairDot();
}

// Called every frame
void ACrosshair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CrosshairDot)
		SetDotAtRightLocation();
}

void ACrosshair::SetDotAtRightLocation()
{
	FHitResult outHit{};

	const FVector& start = GetActorLocation();
	const FVector end = GetActorLocation() + GetActorForwardVector() * BeamLength;

	//Raycast forwards
	//See what has been hit by the raycast
	if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, CollisionChannel))
	{
		//Put the crosshair dot on that location
		CrosshairDot->SetWorldLocation(outHit.Location);

		CollidedObject = outHit.GetActor();
	}
	else
	{
		//Put the crosshair far away in the distance
		CrosshairDot->SetWorldLocation(end * 1000.f);

		CollidedObject = nullptr;
	}
}

UStaticMeshComponent* ACrosshair::GetCrosshairDot() const 
{
	TArray<UActorComponent*> actorComponents = GetComponentsByTag(UStaticMeshComponent::StaticClass(), "CrosshairDot");

	for (UActorComponent* crosshairDot : actorComponents)
		return Cast<UStaticMeshComponent>(crosshairDot);

	UE_LOG(LogTemp, Warning, TEXT("No mesh with tag CrosshairDot found on a crosshair."));
	return nullptr;
}