// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "CasterEnemy.generated.h"

class AEnemyShootProjectile;
class UAudioComponent;

/**
 * 
 */
UCLASS()
class AURORA_API ACasterEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public:
	ACasterEnemy();

	virtual void Tick(float DeltaTime) override;
protected:
	void BeginPlay() override;

private:
	void CheckIsTriggered() override;

	void ShootProjectile();

	void DeleteAttachedProjectile();

	void SpecialAbility();

	FTimerHandle ShootTimer;

	AActor* PlayerRef;

	UAudioComponent* ShootSound;

	UPROPERTY(EditDefaultsOnly, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		float FireRate = 7.f;
	UPROPERTY(EditDefaultsOnly, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		float FireDistance = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ShootAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AEnemyShootProjectile> ProjectileType;
};
