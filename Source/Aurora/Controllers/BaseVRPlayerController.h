// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseVRPlayerController.generated.h"

class ABaseVRCharacter;

/**
 * 
 */


UENUM(BlueprintType)
enum class EGrip : uint8
{
	Open,
	Point,
	Grab
};

UCLASS()
class AURORA_API ABaseVRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;

	void StopDrawing(bool bLeftController);
protected:
	virtual void BeginPlay() override;
private:

	void LeftTriggerPressed();
	void LeftTriggerReleased();

	void RightTriggerPressed();
	void RightTriggerReleased();

	void LeftHoldPressed();
	void RightHoldPressed();
	void LeftHoldReleased();
	void RightHoldReleased();

	void ActionButtonPressed();
	void ActionButtonReleased();

	//If activate is false then we deactivate the ability
	void ActivateAbility(bool bActivate);

	void StartDrawing(bool bLeftController);

	ABaseVRCharacter* Player;

	bool bLeftDrawing;
	bool bRightDrawing;
	bool bInGame;


	EGrip LeftGripValue = EGrip::Open;
	EGrip RightGripValue = EGrip::Open;

	void UpdateLeftGrip(EGrip NewGrip, bool IsGrabOpen);
	void UpdateRightGrip(EGrip NewGrip, bool IsGrabOpen);

	UFUNCTION(BlueprintPure)
		EGrip GetLeftGripValue() const;

	UFUNCTION(BlueprintPure)
		EGrip GetRightGripValue() const;
};
