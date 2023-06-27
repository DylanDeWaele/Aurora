// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Aurora/Controllers/BaseEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UHealthComponent::IncreaseHealth(int32 amount)
{
	int32 health = CurrentHealth + amount;

	CurrentHealth = FMath::Min(MaxHealth, health);
}

void UHealthComponent::DecreaseHealth(int32 amount)
{
	CurrentHealth -= amount;

	if (CurrentHealth <= 0)
	{
		bIsDead = true;
		SetIsDeadInBlackBoard();
	}
}

bool UHealthComponent::IsDead() const
{
	return bIsDead;
}
int32 UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthPercentage() const
{
	return CurrentHealth / (float)MaxHealth;
}

void UHealthComponent::SetIsDeadInBlackBoard()
{
	auto controller = GetOwner()->GetInstigatorController<ABaseEnemyController>();

	if (controller)
	{
		auto blackboard = controller->GetBlackboardComponent();
		if (blackboard)
		{
			blackboard->SetValueAsBool("IsDead", true);
		}
	}
}

