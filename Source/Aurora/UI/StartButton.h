// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/UI/BaseButton.h"
#include "StartButton.generated.h"

class UAudioComponent;
class ABaseVRCharacter;

UCLASS()
class AURORA_API AStartButton : public ABaseButton
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStartButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ActivateButton() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool bStartSoundPlayed = false;
	UAudioComponent* StartSound;

	ABaseVRCharacter* Player;
};
