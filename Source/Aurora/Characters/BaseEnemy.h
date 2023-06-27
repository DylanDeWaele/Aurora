// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Aurora/Enums/EnemyType.h"
#include "BaseEnemy.generated.h"

class UHealthComponent;
class UKillableComponent;
class UPurifiableComponent;
class UTurnableComponent;
class UNiagaraComponent;
class USphereComponent;
class AAudioManager;
class UNiagaraSystem;

enum class EEnemyType : uint8;

UCLASS()
class AURORA_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	void SetEnemyPath(const TArray<FVector>& locations);

	void SetEnemyType(EEnemyType Type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetTriggerBlackBoardValue(bool Enable);

	//boolean used for special ability of each enemy
	bool IsTriggered = false;

	void InitPurifiableComponent();
	void InitTurnableComponent();

	AAudioManager* AudioManager;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
		EEnemyType GetEnemyType() const;

	UFUNCTION(BlueprintPure)
		bool GetIsTriggered() const;

	UFUNCTION(BlueprintPure)
		bool GetHasEnemy() const;


	UHealthComponent* GetHealthComponent() const;
	UPurifiableComponent* GetPurifiableComponent() const;
	UTurnableComponent* GetTurnableComponent() const;

	void GetHit(int32 HitAnimIndex, int32 Damage);

	void PlayEvaporateGoodEffect();
	void PlayEvaporateEvilEffect();

	void TurnColliders(bool Enable);

	void StopBreathing();

	UFUNCTION(BlueprintPure)
		bool IsBeingEvaporated() const;

private:
	virtual void CheckIsTriggered();

	void CheckIsDead();

	//locations of the path he is taking
	TArray<FVector> Locations;

	//components
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UKillableComponent* KillableComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USphereComponent* EnemyDetectionSphere;

	//Purify particles
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* PurifyImpactComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* PurifiedEffectComponent;

	//Evaporate particles
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* EvaporateEvilEffectComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* EvaporateGoodEffectComponent;


	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* DeathCloud;
	void SetupEvaporateParticles();

	EEnemyType EnemyType = EEnemyType::None;

	UPurifiableComponent* PurifiableComponent;
	UTurnableComponent* TurnableComponent;

	UAudioComponent* BreathingSound;

	bool bIsBeingEvaporated = false;

	//AI
	ABaseEnemy* Target;

	void SetEnemyInBlackboard(ABaseEnemy* Enemy);
	void SetupDetectionSphere();

	UFUNCTION(BlueprintCallable)
		void AttackEnemyTarget(int32 HitAnimIndex, int32 Damage);

	UFUNCTION(BlueprintCallable)
		void EvilEvaporateSelf();

	UFUNCTION(BlueprintCallable)
		void GoodEvaporateSelf();

		FTimerHandle KillSelf;

		void DestroySelf();

	//Array of hit animations
	UPROPERTY(EditDefaultsOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> GetHitAnims;

	void InitPlayerHealthRef();

	UHealthComponent* PlayerHealthRef;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		int32 EvaporateDamage = 10;

	AAudioManager* GetAudioManager() const;
};
