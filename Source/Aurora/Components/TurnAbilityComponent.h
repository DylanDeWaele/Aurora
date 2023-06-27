// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/Components/BaseAbilityComponent.h"
#include "TurnAbilityComponent.generated.h"

class ATurnBomb;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AURORA_API UTurnAbilityComponent : public UBaseAbilityComponent
{
	GENERATED_BODY()

public:
	UTurnAbilityComponent();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateAbility() override;

private:
	UPROPERTY(EditAnywhere, category = "Turn", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATurnBomb> TurnBomb;

	ATurnBomb* SpawnedBomb = nullptr;
	USceneComponent* SpawnSocket = nullptr;

	USceneComponent* GetSpawnSocket() const;
};
