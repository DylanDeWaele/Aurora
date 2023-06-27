// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/Components/BaseAbilityComponent.h"
#include "PurifyAbilityComponent.generated.h"

class APurifyBeam;
class AAudioManager;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AURORA_API UPurifyAbilityComponent : public UBaseAbilityComponent
{
	GENERATED_BODY()

public:
	UPurifyAbilityComponent();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void ActivateAbility() override;
	virtual void DeactivateAbility() override;

private:
	UPROPERTY(EditAnywhere, Category = "Purification", meta = (AllowPrivateAccess = "true"))
		float DrainSpeed = 20.f; //Every second, when purifying, the ammo count will decrease by the drain speed => means we have 3 seconds of draintime

	UPROPERTY(EditAnywhere, Category = "Purification", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APurifyBeam> PurifyBeamClass;

	bool bIsPurifying = false;

	APurifyBeam* PurifyBeam;

	AAudioManager* AudioManager;
	UAudioComponent* ShootSound;

	AAudioManager* GetAudioManager() const;
};