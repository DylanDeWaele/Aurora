// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//My includes
#include "Aurora/Enums/CrosshairType.h"

#include "GameFramework/Actor.h"
#include "Crosshair.generated.h"

UCLASS()
class AURORA_API ACrosshair : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACrosshair();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	ECollisionChannel CollisionChannel;

	AActor* CollidedObject;

private:
	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
		float BeamLength = 10000.f;

	UStaticMeshComponent* CrosshairDot;

	UStaticMeshComponent* GetCrosshairDot() const;

	void SetDotAtRightLocation();
};