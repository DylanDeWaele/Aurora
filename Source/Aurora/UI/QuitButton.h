// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/UI/BaseButton.h"
#include "QuitButton.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API AQuitButton : public ABaseButton
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuitButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ActivateButton() override;
};
