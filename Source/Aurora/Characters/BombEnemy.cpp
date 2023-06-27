// Fill out your copyright notice in the Description page of Project Settings.


#include "BombEnemy.h"
#include "Aurora/Components/HealthComponent.h"
#include "Components/SphereComponent.h"

ABombEnemy::ABombEnemy()
{
	ExplosionHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionHitBox"));
	ExplosionHitBox->SetupAttachment(GetRootComponent());
}

void ABombEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsTriggered != true)
		CheckIsTriggered();
}

void ABombEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthRef = Cast<UHealthComponent>(GetComponentByClass(UHealthComponent::StaticClass()));
	if(!HealthRef)
		UE_LOG(LogTemp, Warning, TEXT("healthRef not found on bomber"));
}

void ABombEnemy::CheckIsTriggered()
{
	if (IsValid(ExplosionMontage) && IsValid(HealthRef))
	{
		if (HealthRef->IsDead())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(ExplosionMontage);

			IsTriggered = true;
			SetTriggerBlackBoardValue(true);
		}
	}
}
