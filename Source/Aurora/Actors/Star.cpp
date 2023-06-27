// Fill out your copyright notice in the Description page of Project Settings.


#include "Star.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraComponent.h"

#define ECC_STARDRAWING ECC_GameTraceChannel1


// Sets default values
AStar::AStar()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(Collider);

	//StarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StarMesh"));
	//StarMesh->SetupAttachment(GetRootComponent());

	StartPointMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("StartPointMaterial"));
	DefaultMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("DefaultMaterial"));

	LitParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LitParticles"));
	DefaultParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DefaultParticles"));
	StartPointParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("StartPointParticles"));

	LitParticles->SetupAttachment(Collider);
	DefaultParticles->SetupAttachment(Collider);
	StartPointParticles->SetupAttachment(Collider);
}

// Called when the game starts or when spawned
void AStar::BeginPlay()
{
	Super::BeginPlay();

	SetStarCollision();
}

// Called every frame
void AStar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStar::SetStarCollision()
{
	if (Collider)
	{
		//set simulate hit events
		Collider->SetCollisionObjectType(ECC_WorldStatic);
		Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Collider->SetCollisionResponseToChannel(ECC_STARDRAWING, ECR_Block);
	}
}

void AStar::SetStarIndex(int32 Index)
{
	UniqueIndex = Index;
}

int32 AStar::GetStarIndex() const
{
	return UniqueIndex;
}

void AStar::Activate()
{
	bIsLit = true;

	//StarMesh->SetScalarParameterValueOnMaterials("emissive", Litness);

	if (bIsStart)
		StartPointParticles->SetVisibility(false);
	else
		DefaultParticles->SetVisibility(false);

	LitParticles->SetVisibility(true);
}

void AStar::DeActivate()
{
	bIsLit = false;

	//StarMesh->SetScalarParameterValueOnMaterials("emissive", 1.f);

	if (bIsStart)
		StartPointParticles->SetVisibility(true);
	else
		DefaultParticles->SetVisibility(true);

	LitParticles->SetVisibility(false);
}

bool AStar::IsLit() const
{
	return bIsLit;
}

void AStar::MarkAsStart()
{
	//if (StarMesh && StartPointMaterial)
	//{
	//	StarMesh->SetMaterial(0, StartPointMaterial);
	//}
	
	bIsStart = true;

	StartPointParticles->SetVisibility(true);
	DefaultParticles->SetVisibility(false);
}

void AStar::UnMarkAsStart()
{
	//if (StarMesh && DefaultMaterial)
	//{
	//	StarMesh->SetMaterial(0, DefaultMaterial);
	//}

	bIsStart = false;

	StartPointParticles->SetVisibility(false);
	DefaultParticles->SetVisibility(true);
}