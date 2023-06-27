// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//My includes
#include "Aurora/Enums/Ability.h"
#include "Aurora/Enums/CrosshairType.h"

#include "Components/ActorComponent.h"
#include "AbilityManagerComponent.generated.h"


//Class forward declarations
class UBaseAbilityComponent;
class UKillAbilityComponent;
class UPurifyAbilityComponent;
class UTurnAbilityComponent;
class ABaseVRCharacter;
class ACrosshairManager;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AURORA_API UAbilityManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void SetAbility(EAbility NewAbility);
	ECrosshairType GetCrosshairOfCurrentAbility() const;
	EAbility GetCurrentAbility() const;

	float GetCurrentAbilityAmmoPercentage() const;

	void ActivateCurrentAbility();

	void DeactivateCurrentAbility();

private:
	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
		EAbility CurrentAbility;

	//Ability components
	UBaseAbilityComponent*   CurrentAbilityComponent;
	UKillAbilityComponent*   KillAbilityComponent;
	UPurifyAbilityComponent* PurifyAbilityComponent;
	UTurnAbilityComponent*   TurnAbilityComponent;

	//Owner - Turret or Player
	AActor* Owner;

	UKillAbilityComponent* GetKillAbilityComponent() const;
	UPurifyAbilityComponent* GetPurifyAbilityComponent() const;
	UTurnAbilityComponent* GetTurnAbilityComponent() const;

	UActorComponent* FindAbilityComponent(EAbility AbilityType) const;
	void DisableCrosshair(ACrosshairManager* Crosshair, ABaseVRCharacter* Player);
};
