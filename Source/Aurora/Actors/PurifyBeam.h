// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PurifyBeam.generated.h"

class UNiagaraComponent;
class USplineMeshComponent;
class UBoxComponent;

UCLASS()
class AURORA_API APurifyBeam : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APurifyBeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EnableBeam(bool bEnableBeam);

	bool BeamEnabled() const;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* Collider;
	  
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* Particles;

	bool bEnabled = false;
};