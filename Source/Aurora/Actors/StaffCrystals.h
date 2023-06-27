// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Aurora/Enums/Ability.h"
#include "StaffCrystals.generated.h"

class UMaterialInterface;
class UMaterialInstanceDynamic;
class ABaseVRCharacter;

UCLASS()
class AURORA_API AStaffCrystals : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStaffCrystals();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetAbility(EAbility Type);

private:
	//Meshes
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BigCrystal;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* SmallCrystals;

	//Materials
	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* NeutralCrystalMaterial;
	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* KillCrystalMaterial;
	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* PurifyCrystalMaterial;

	UMaterialInstanceDynamic* SmallCrystalDynamicMaterial = nullptr;
	UMaterialInstanceDynamic* BigCrystalDynamicMaterial = nullptr;

	ABaseVRCharacter* Player;
	EAbility CurrentAbility;

	void UpdateMaterial();
};
