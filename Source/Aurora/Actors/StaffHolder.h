// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaffHolder.generated.h"

class ABaseVRCharacter;

UCLASS()
class AURORA_API AStaffHolder : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStaffHolder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		USceneComponent* StaffSocket;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float TeleportDistance = 150.f;

	AActor* Staff;
	ABaseVRCharacter* Player;

	bool StaffOutOfRange() const;
	void TeleportStaffBack();
};
