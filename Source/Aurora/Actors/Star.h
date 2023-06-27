// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Star.generated.h"

class UPointLightComponent;
class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;

UCLASS()
class AURORA_API AStar : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStar();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStarIndex(int32 Index);
	int32 GetStarIndex() const;

	void Activate();

	void DeActivate();

	bool IsLit() const;

	void MarkAsStart();
	void UnMarkAsStart();

	FTransform GetCompletionPlaceholderTransform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USphereComponent* Collider;
	//UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	//	UStaticMeshComponent* StarMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Material", meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* StartPointMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Material", meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Particles", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* DefaultParticles;
	UPROPERTY(EditDefaultsOnly, Category = "Particles", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* LitParticles;
	UPROPERTY(EditDefaultsOnly, Category = "Particles", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* StartPointParticles;

	void SetStarCollision();

	int32 UniqueIndex;

	bool bIsLit = false;
	bool bIsStart = false;

	UPROPERTY(EditAnywhere, Category = "StarSpecifications", meta = (AllowPrivateAccess = "true"))
		float Litness = 100.f;
};
