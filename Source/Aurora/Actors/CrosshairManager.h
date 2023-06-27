// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//My includes
#include "Aurora/Enums/CrosshairType.h"

#include "GameFramework/Actor.h"
#include "CrosshairManager.generated.h"

class AMenuCrosshair;

UCLASS()
class AURORA_API ACrosshairManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACrosshairManager();

	void SetCrosshair(ECrosshairType CrosshairType);
	ECrosshairType GetCurrentCrosshair() const;
	AMenuCrosshair* GetMenuCrosshair() const;

	void TurnCrosshairOn();
	void TurnCrosshairOff();

private:
	//Crosshairs
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* StarDrawingCrosshair;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* KillAbilityCrosshair;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* MenuCrosshair;

	//Current crosshair
	UChildActorComponent* CurrentCrosshairActor = nullptr;
	ECrosshairType CurrentCrosshair;
};