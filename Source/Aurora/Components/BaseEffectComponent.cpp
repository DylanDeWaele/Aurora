// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEffectComponent.h"
#include "StatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "Aurora/Characters/BaseVRCharacter.h"

// Sets default values for this component's properties
UBaseEffectComponent::UBaseEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UBaseEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	StatsComponent = GetStatsComponent();
	Owner = Cast<ABaseEnemy>(GetOwner());
}

void UBaseEffectComponent::AddToStats(EAbility StatType, int32 Amount)
{
	if (StatsComponent)
	{
		//TODO: Implement thi1s
		switch (StatType)
		{
		case EAbility::Kill:
			//Add amount to kills stat
			StatsComponent->AddEnemiesKilled(Amount);
			break;
		case EAbility::Purify:
			//Add amount to purify 
			StatsComponent->AddEnemiesPurified(Amount);
			break;
		case EAbility::Turn:
			//Add amount to turn
			StatsComponent->AddEnemiesTurned(Amount);
			break;
		}
	}
}

UStatsComponent* UBaseEffectComponent::GetStatsComponent() const
{
	ABaseVRCharacter* player = Cast<ABaseVRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (player)
	{
		return Cast<UStatsComponent>(player->GetStatsComponent());

		UE_LOG(LogTemp, Warning, TEXT("No stats component found on the player."));
		return nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("No player found while trying to get the stats component in an effect class."));
	return nullptr;
}
