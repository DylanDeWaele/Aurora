// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/Components/BaseAbilityComponent.h"
#include "KillAbilityComponent.generated.h"

class AKillProjectile;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AURORA_API UKillAbilityComponent : public UBaseAbilityComponent
{
	GENERATED_BODY()

public:
	UKillAbilityComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void ActivateAbility() override;

private:
	//Editor variables
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AKillProjectile> KillProjectileClass;

	//Helper functions
	void ShootProjectile() const;
};
