// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/UI/BaseButton.h"
#include "BackButton.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API ABackButton : public ABaseButton
{
	GENERATED_BODY()

public:
	// Called when the game starts or when spawned
	ABackButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ActivateButton() override;
};
