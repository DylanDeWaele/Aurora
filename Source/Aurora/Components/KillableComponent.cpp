// Fill out your copyright notice in the Description page of Project Settings.


#include "KillableComponent.h"
#include "Aurora/Components/HealthComponent.h"
#include "Aurora/Components/PurifiableComponent.h"
#include "Aurora/Components/TurnableComponent.h"
#include "Aurora/Characters/BaseEnemy.h"

UKillableComponent::UKillableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UKillableComponent::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent = GetHealthComponent();
}

void UKillableComponent::Damage(int32 amount)
{
	UPurifiableComponent* purifiableComponent = Owner->GetPurifiableComponent();
	UTurnableComponent* turnableComponent = Owner->GetTurnableComponent();
	bool beingEvaporated = Cast<ABaseEnemy>(Owner)->IsBeingEvaporated();

	if (!beingEvaporated && !turnableComponent->IsTurned() && !purifiableComponent->IsPurified())
	{
		if (HealthComponent)
		{
			HealthComponent->DecreaseHealth(amount);
			CheckIfDead();
		}
	}
}

UHealthComponent* UKillableComponent::GetHealthComponent() const
{
	UActorComponent* healthComponent = Owner->GetComponentByClass(UHealthComponent::StaticClass());

	if (healthComponent)
		return Cast<UHealthComponent>(healthComponent);

	UE_LOG(LogTemp, Warning, TEXT("No associated health component found on an object that has a killable component on it."));
	return nullptr;
}


void UKillableComponent::CheckIfDead()
{
	if (HealthComponent->IsDead())
	{
		AddToStats(EAbility::Kill, 1); //Adding count to the kill stat in the players stats component
	}
}