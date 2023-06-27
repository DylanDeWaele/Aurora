// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/Components/BaseEffectComponent.h"
#include "KillableComponent.generated.h"

class UHealthComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AURORA_API UKillableComponent : public UBaseEffectComponent
{
	GENERATED_BODY()
	
public:
	UKillableComponent();

protected:
	virtual void BeginPlay() override;

public:
	void Damage(int32 amount);

private:
	UHealthComponent* HealthComponent;

	//Helper functions
	UHealthComponent* GetHealthComponent() const;
	void CheckIfDead();
};
