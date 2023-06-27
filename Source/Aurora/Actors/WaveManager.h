// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

enum class EEnemyType : uint8;

class ABaseEnemy;
class ABaseEnemySpawner;
class AAudioManager;
class UAudioComponent;
class UNiagaraSystem;

UCLASS()
class AURORA_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FadeOutDrums();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<AActor*> Spawners;

	//TIMERS
	FTimerHandle CountDownTimer;
	FTimerHandle SpawnTimer;
	FTimerHandle WaveTimer;
	FTimerHandle EnemiesDeadTimer;

	UPROPERTY(EditAnywhere, Category = "Timers", meta = (AllowPrivateAccess = "true"))
		int32 CountDownTime = 5.f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"),Category = "Timers")
		int32 SpawnTime = 5.f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Timers")
		int32 WaveTime = 20.f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Timers")
		int32 AddedWaveTime = 10.f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Timers")
		int32 DecreasedSpawnTime = 2.f;

	//MAIN WAVE SYSTEM
	void CountDown();
	void StartWave();
	void EndWave();
	void Spawn();

	//ON NEW WAVE
	void IncreaseWaveDifficulty();
	void SetEnemyArray();
	void DecreaseSpawnTime();
	void ChangeEnemyPercentages();
	void IncreaseAmountOfSpawnsPerSpawn();
	EEnemyType const GetRandomEnemyType();
	ABaseEnemySpawner* GetRandomSpawnPoint();


	static int32 WaveNumber;

	//array that defines the chances of which enemy will be spawn
	TArray<EEnemyType> EnemiesForThisWave;

	//enemies to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyType", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABaseEnemy> TankEnemy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyType", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABaseEnemy> BombEnemy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyType", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABaseEnemy> AssassinEnemy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyType", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABaseEnemy> CastEnemy;

	TSubclassOf<ABaseEnemy> EnemyToSpawn;


	//togheter has to be 100
	UPROPERTY(EditAnywhere, Category = "ChancePercentages", meta = (AllowPrivateAccess))
		int32 TankChancePercentage = 30;
	UPROPERTY(EditAnywhere, Category = "ChancePercentages", meta = (AllowPrivateAccess))
		int32 BombChancePercentage = 30;
	UPROPERTY(EditAnywhere, Category = "ChancePercentages", meta = (AllowPrivateAccess))
		int32 AssassinChancePercentage = 20;
	UPROPERTY(EditAnywhere, Category = "ChancePercentages", meta = (AllowPrivateAccess))
		int32 CasterChancePercentage = 20;

	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (AllowPrivateAccess))
		int32 MaxAmountOfEnemiesPerSpawn = 1;
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
		int32 AmountOfRoundsToDecreaseSpawnTimer = 4;
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
		int32 MinimumSpawnerTime = 4.f;
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
		int32 AmountOfRoundsToIncreaseAmountOfSpawns = 5;
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
		int32 AmountOfRoundsToChangePercentages = 5;
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
		int32 PercentageChange = 3;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* DeathCloud;

	AAudioManager* AudioManager;
	UAudioComponent* CurrentDrumSound;
	float CurrentDrumVolume = 0.0f;

	void CheckIfAllEnemiesAreDead();

	AAudioManager* GetAudioManager() const;
};
