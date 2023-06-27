// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//My includes
#include "Aurora/Enums/Ability.h"

#include "Components/ActorComponent.h"
#include "BaseEffectComponent.generated.h"

class UStatsComponent;
class ABaseEnemy;

UCLASS( )
class AURORA_API UBaseEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseEffectComponent();

protected:
	virtual void BeginPlay() override;

	//Add an amount to a specific stat like amount of kills/purifies/turns
	void AddToStats(EAbility StatType, int32 Amount);

	//These effects should only be applied to actors of type enemy
	ABaseEnemy* Owner;

private:
	UStatsComponent* StatsComponent;

	UStatsComponent* GetStatsComponent() const;
};
