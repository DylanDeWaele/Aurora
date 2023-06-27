// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "BombEnemy.generated.h"

class UHealthComponent;
class USphereComponent;

UCLASS()
class AURORA_API ABombEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public:
	ABombEnemy();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
private:

	void CheckIsTriggered() override;

	UHealthComponent* HealthRef;

	UPROPERTY(VisibleAnywhere, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		USphereComponent* ExplosionHitBox;

	UPROPERTY(EditAnywhere, Category = "Trigger", meta = (AllowPrivateAccess = "true"), BlueprintReadOnly)
		int32 ExplosionDamage = 20;

	UPROPERTY(EditAnywhere, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ExplosionMontage;
};
