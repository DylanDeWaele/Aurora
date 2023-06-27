// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAbilityComponent.generated.h"
 
class UArrowComponent;

UCLASS()
class AURORA_API UBaseAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseAbilityComponent();

	virtual void ActivateAbility() PURE_VIRTUAL(&UBaseAbilityComponent::ActivateAbility, ;); //Pure virtual function
	virtual void DeactivateAbility() PURE_VIRTUAL(&UBaseAbilityComponent::DeactivateAbility, ;); //Pure virtual function

	float GetCurrentAmmoPercentage() const;

	void RefillAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
		float MaxAmmo;

	float CurrentAmmo = 0.f;

	UArrowComponent* ShootSocket;

	AActor* Owner;
	AActor* Staff;

	//Helper functions
	bool CheckNoAmmo();
	UArrowComponent* FindShootSocket() const;
};