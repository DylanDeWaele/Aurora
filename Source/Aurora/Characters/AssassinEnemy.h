// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "Components/TimelineComponent.h"
#include "AssassinEnemy.generated.h"
/**
 * 
 */
UCLASS()
class AURORA_API AAssassinEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public:
	AAssassinEnemy();

	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
private:
	void CheckIsTriggered() override;

	UPROPERTY(VisibleAnywhere, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		USphereComponent* ProjectileDetectionBox;
	UPROPERTY(EditAnywhere, Category = "Trigger", meta = (AllowPrivateAccess = "true"), BlueprintReadOnly)
		float DashSpeed = 4000.f;
	UPROPERTY(EditAnywhere, Category = "Trigger", meta = (AllowPrivateAccess = "true"), BlueprintReadOnly)
		float DashAcceleration = 20000.f;
	UPROPERTY(EditAnywhere, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DashAnim;
	UPROPERTY(EditAnywhere, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		float CooldownTime = 5.f;

	float RemainingTime = 0.0f;

	bool ShouldDash();
	void OnMontageEnded(UAnimMontage* Montage, bool bInteruppted);

	//TIMELINE//
	class UTimelineComponent* DashTimeline;

	//used to init the timeline
	UPROPERTY(EditAnywhere, Category = "trigger", meta = (AllowPrivateAccess = "true"))
		class UCurveFloat* fCurveDash;

	//Delegates
	FOnTimelineFloat UpdateTimeline{};

	FOnTimelineEvent FinishedTimeline{};

	//update of the timeline
	UFUNCTION()
		void TimelineFloatReturn(float value);
	//finished the timeline
	UFUNCTION()
		void OnTimelineFinished();

	FVector DashDirection;

};
