// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnBomb.generated.h"

class UPickupableComponent;
class USphereComponent;
class UNiagaraComponent;
class AAudioManager;

UCLASS()
class AURORA_API ATurnBomb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurnBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsThrown() const;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BombMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* BombParticles;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		USphereComponent* AreaOfEffect;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* AreaOfEffectParticles;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UPickupableComponent* Pickupable;

	UPROPERTY(EditAnywhere, Category = "Turn", meta = (AllowPrivateAccess = "true"))
		float TimeActive = 10.f;

	UPROPERTY(EditAnywhere, Category = "Turn", meta = (AllowPrivateAccess = "true"))
		float ScaleDownSpeed = 0.1f;

	bool bIsThrown = false;
	bool bThrowSoundPlayed = false;
	bool bExploded = false;

	AAudioManager* AudioManager;
	UAudioComponent* HoldSound;

	FVector CurrentScale;
	FTimerHandle TimerHandle;

	void Explode();

	void ScaleDown(float DeltaTime);

	void SetExploded();

	AAudioManager* GetAudioManager() const;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
