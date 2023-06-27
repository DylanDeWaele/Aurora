// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Aurora/Actors/Crosshair.h"
#include "MenuCrosshair.generated.h"

UCLASS()
class AURORA_API AMenuCrosshair : public ACrosshair
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMenuCrosshair();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PressButton();
	void ReleaseButton();
};
