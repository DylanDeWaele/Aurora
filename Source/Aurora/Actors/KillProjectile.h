// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class AAudioManager;

UCLASS()
class AURORA_API AKillProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKillProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& Direction);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		USphereComponent* ColliderComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* ParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Impact" , meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* ImpactParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		int32 DamageAmount = 20;

	//pointer to the audio manager, we need this in order to play sounds
	AAudioManager* AudioManager;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	AAudioManager* GetAudioManager() const;
};
