// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/UI/BaseButton.h"
#include "Aurora/Actors/AudioManager.h"
#include "DecrementVolumeButton.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API ADecrementVolumeButton : public ABaseButton
{
	GENERATED_BODY()
	
public:
	// Called when the game starts or when spawned
	ADecrementVolumeButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ActivateButton() override;
	virtual void DeactivateButton() override;

private:
	UPROPERTY(EditAnywhere, Category = "Type", meta = (AllowPrivateAccess = "true"))
		EType VolumeType;

	FTimerHandle TimerHandle;
	float Interval = 0.1f;

	void DecrementVolume();
};
