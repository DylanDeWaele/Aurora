// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "TankEnemy.generated.h"


UCLASS()
class AURORA_API ATankEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ATankEnemy();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
private:
	void CheckIsTriggered() override;

	UPROPERTY(EditAnywhere, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		float EnrageDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		float EnragedSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* RoarAnimation;

	AActor* PlayerRef;

	void OnMontageEnded(UAnimMontage* Montage, bool bInteruppted);
};
