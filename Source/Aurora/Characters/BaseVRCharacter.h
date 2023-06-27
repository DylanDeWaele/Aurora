// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Aurora/Actors/Staff.h"
#include "BaseVRCharacter.generated.h"

class UCameraComponent;
class USphereComponent;
class UMotionControllerComponent;
class UAudioComponent;

class AStarField;
class AStar;
class AAudioManager;
class AAuroraGameModeBase;

class UAbilityManagerComponent;
class ACrosshairManager;

class UKillAbilityComponent;
class UPurifyAbilityComponent;
class UTurnAbilityComponent;

class UStatsComponent;
class UHealthComponent;
class UPostProcessComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS()
class AURORA_API ABaseVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseVRCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Function to get the crosshair actor of the player -> Handy for quickly setting crosshairs
	ACrosshairManager* GetLeftCrosshair() const;
	ACrosshairManager* GetRightCrosshair() const;

	UStatsComponent* GetStatsComponent() const;
	UHealthComponent* GetHealthComponent() const;

	UAudioComponent* GetHeartbeat() const;

	bool LeftHandHoldingStaff() const;
	bool RightHandHoldingStaff() const;

	void Drawing(bool bLeftController);
	void StopDrawing(bool bLeftController);

	void GrabActor(bool bLeftController);
	void ReleaseActor(bool bLeftController);

	//This function will check if we are hitting a button with our crosshair and then press it (handled inside the menu crosshair class)
	void PressButton(bool bLeftController);
	void ReleaseButton(bool bLeftController);

	void ShowStats(bool bShowStats);

	void FadeIn();
	void FadeOut();
	bool ScreenIsBlack() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentAmmoPercentage() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* VRRoot;

	//Hands
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UMotionControllerComponent* LeftController;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UMotionControllerComponent* RightController;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USphereComponent* LeftGrabSphere;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USphereComponent* RightGrabSphere;

	//Abilities
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UAbilityManagerComponent* AbilityManager;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UKillAbilityComponent* KillAbiity;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UPurifyAbilityComponent* PurifyAbility;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UTurnAbilityComponent* TurnAbility;

	//Stats
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStatsComponent* StatsComponent;

	//Health
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UHealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UPostProcessComponent* BlackScreenEffect;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "True"), Category = "Purification")
		UMaterialInterface* BlackScreenMaterial;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "True"), Category = "Purification")
		UMaterialInterface* PlayerHealthVignette;

	//vignette
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "True"), Category = "Vignette")
		UStaticMeshComponent* PlaneVignette;
		
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "True"), Category = "Vignette")
		float HealthVignetteValue = 0.6f;

	UMaterialInstanceDynamic* PlayerHealthVignetteDynamicMaterial;
	//Stats
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* PlayerStatsActor;

	UMaterialInstanceDynamic* BlackScreenEffectDynamicMaterial;
	float CurrentBlackScreenTransparency = 0;
	float FadeSpeed = 0.25f; //per second, meaning over 3 seconds
	bool bFadingIn = false;
	bool bFadingOut = false;

	AAuroraGameModeBase* GameMode;

	AActor* ActorInLeftHand = nullptr;
	AActor* ActorInRightHand = nullptr;

	AStaff* Staff = nullptr;

	AAudioManager* AudioManager;
	UAudioComponent* CurrentFailSound = nullptr;
	UAudioComponent* HeartbeatSound = nullptr;
	int CurrentHeartbeatIndex = -1;

	bool bLeftHandHoldingStaff = false;
	bool bRightHandHoldingStaff = false;

	bool bCrosshairInitialized = false;

	float SlowMotionValue = 0.5f;

	void OffsetCameraToRoot();

	void LineTraceHMDTowardsStarFields();

	AStar* IsStarHit(bool bLeftController);

	AStarField* CurrentStarField = nullptr;

	//With the bLeft boolean you can choose from which hand you want to get the crosshair manager from
	ACrosshairManager* GetCrosshairManager(bool bLeft = true) const;

	void HandleStarFields(AStarField* HitStarField);

	void ProduceAbility();

	//Get Closest actor that has a pickup able components
	AActor* GetActorNearHand(bool bLeftController);

	void GrabStaff(bool bLeftController);
	void GrabObject(bool bLeftController, AActor* ActorNearHand);

	void ReleaseStaff(bool bLeftController);
	void ReleaseObject(bool bLeftController);

	FStaffSocket* GetStaffSocketInRange(USphereComponent* GrabSphere) const;

	void ReparentBackSocket();

	void DropStaff();

	void InitializePostProcessingEffect();

	void SetupCrosshair();

	void HandleHeartbeart();

	void HandleHealthVignette();
	
	void HandleFading(float DeltaTime);

	AAudioManager* GetAudioManager() const;
};
