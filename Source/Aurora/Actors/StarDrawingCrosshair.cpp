// Fill out your copyright notice in the Description page of Project Settings.


#include "StarDrawingCrosshair.h"
#include "DrawDebugHelpers.h"

// Sets default values
AStarDrawingCrosshair::AStarDrawingCrosshair()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionChannel = ECC_GameTraceChannel1;
}

// Called when the game starts or when spawned
void AStarDrawingCrosshair::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStarDrawingCrosshair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}