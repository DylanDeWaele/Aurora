// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Aurora/Enums/Ability.h"
#include "Staff.generated.h"

class UMotionControllerComponent;
class ACrosshairManager;
class UNiagaraComponent;
class AStaffCrystals;
class AAudioManager;

USTRUCT()
struct AURORA_API FStaffSocket
{
	GENERATED_USTRUCT_BODY();

public:
	USceneComponent* Component;
	UMotionControllerComponent* Controller = nullptr;

	bool bIsBack = false;
	bool bLeftController = false;
};

UCLASS()
class AURORA_API AStaff : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStaff();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetAbility(EAbility Type);

	ACrosshairManager* GetCrosshair() const;

	FStaffSocket FrontSocket;
	FStaffSocket BackSocket;

private:
	//Meshes
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* Crystals;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* DecorationOne;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* DecorationTwo;

	//Sockets
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* FrontSocketComponent;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BackSocketComponent;

	//Particle systems
	UNiagaraComponent* CurrentParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* NeutralParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* KillAbilityParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* PurifyAbilityParticleSystemComponent;

	UStaticMeshComponent* FrontSocketLeftHand;
	UStaticMeshComponent* FrontSocketRightHand;
	UStaticMeshComponent* BackSocketLeftHand;
	UStaticMeshComponent* BackSocketRightHand;

	//Audio manager
	AAudioManager* AudioManager;
	UAudioComponent* KillHoldSound;
	UAudioComponent* PurifyHoldSound;

	void RotateTowardsFrontSocket();
	void HandleSocketDisplay();
	void GetHandMeshes();
	AStaffCrystals* GetCrystals() const;
	AAudioManager* GetAudioManager() const;
};