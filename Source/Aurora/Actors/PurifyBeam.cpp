// Fill out your copyright notice in the Description page of Project Settings.


#include "PurifyBeam.h"
#include "NiagaraComponent.h"

#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Aurora/Components/PurifiableComponent.h"
#include "Aurora/Actors/Staff.h"

// Sets default values
APurifyBeam::APurifyBeam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(root);

	Particles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particles"));
	Particles->SetupAttachment(root);
}

// Called when the game starts or when spawned
void APurifyBeam::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APurifyBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APurifyBeam::EnableBeam(bool bEnableBeam)
{
	bEnabled = bEnableBeam;

	if (bEnableBeam)
		Particles->SetVisibility(true);
	else
		Particles->SetVisibility(false);
}

bool APurifyBeam::BeamEnabled() const
{
	return bEnabled;
}