// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "Kismet/GameplayStatics.h" 
#include "Aurora/Actors/BaseEnemySpawner.h"
#include "Aurora/Actors/AudioManager.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "Aurora/Enums/EnemyType.h"
#include "Aurora/Components/HealthComponent.h"
#include "Aurora/Components/TurnableComponent.h"
#include "Aurora/Components/StatsComponent.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "Components/AudioComponent.h"
#include "NiagaraFunctionLibrary.h"

int32 AWaveManager::WaveNumber = 0;

void AWaveManager::CheckIfAllEnemiesAreDead()
{
	TArray<AActor*> enemies;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), enemies);

	if (enemies.Num() != 0)
	{
		//check if still enemies alive.
		for (auto enemy : enemies)
		{
			auto healthComponent = Cast<UHealthComponent>(Cast<ABaseEnemy>(enemy)->GetComponentByClass(UHealthComponent::StaticClass()));
			auto turnComponent = Cast<UTurnableComponent>(Cast<ABaseEnemy>(enemy)->GetComponentByClass(UTurnableComponent::StaticClass()));
			if (healthComponent && turnComponent)
			{
				//so if there is an enemy not dead yet, the new wave won't start.
				if (!healthComponent->IsDead() && !turnComponent->IsTurned())
				{
					return;
				}
			}
		}
	}

	//delete all the enemy dead bodies
	for (auto deadEnemy : enemies)
	{
		if (DeathCloud)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathCloud, deadEnemy->GetActorLocation());

		//Make sure the breathingsound stops
		Cast<ABaseEnemy>(deadEnemy)->StopBreathing();

		deadEnemy->Destroy();
	}

	UE_LOG(LogTemp, Warning, TEXT("wave ended. all enemies dead"));

	GetWorld()->GetTimerManager().ClearTimer(EnemiesDeadTimer);

	IncreaseWaveDifficulty();

	//set countdown again
	GetWorld()->GetTimerManager().SetTimer(CountDownTimer, this, &AWaveManager::CountDown, CountDownTime, false);

	AudioManager->PlaySound(EType::SFX, "Wave_End");

	if (CurrentDrumVolume < 1.f)
		CurrentDrumVolume += 0.125f; //Slowly build up the volume of the drum sounds

	if (CurrentDrumSound)
	{
		CurrentDrumSound->FadeOut(1.f, 0.f);
		CurrentDrumSound = nullptr;
	}
}

// Sets default values
AWaveManager::AWaveManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();

	//init wave
	WaveNumber = 0;

	TSubclassOf<ABaseEnemySpawner> SpawnerType = ABaseEnemySpawner::StaticClass();


	//get all spawners
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnerType, Spawners);

	//init enemy chance array
	SetEnemyArray();

	//set cooldown timer
	GetWorld()->GetTimerManager().SetTimer(CountDownTimer, this, &AWaveManager::CountDown, CountDownTime, false);

	AudioManager = GetAudioManager();
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentDrumSound && CurrentDrumSound->IsPendingKill())
		CurrentDrumSound = nullptr;
}

void AWaveManager::FadeOutDrums()
{
	if (CurrentDrumSound)
		CurrentDrumSound->FadeOut(3.f, 0);
}

void AWaveManager::CountDown()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave started"));
	StartWave();
}

void AWaveManager::StartWave()
{
	//set timer that indicates how long the wave takes
	GetWorld()->GetTimerManager().SetTimer(WaveTimer, this, &AWaveManager::EndWave, WaveTime, false);

	//set timer for spawning each x amount of seconds
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AWaveManager::Spawn, SpawnTime, true, 0.f);

	//end of wave
	++WaveNumber;

	//Increment player wave count
	Cast<UStatsComponent>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetComponentByClass(UStatsComponent::StaticClass()))->AddWavesSurvived(1);

	AudioManager->PlaySound(EType::SFX, "Wave_Start");

	if (CurrentDrumSound)
		CurrentDrumSound->Stop();

	CurrentDrumSound = AudioManager->PlaySound(EType::SFX, "Wave_Drums", 1.5f);
	CurrentDrumSound->SetVolumeMultiplier(CurrentDrumVolume);
}

void AWaveManager::EndWave()
{
	//don't spawn anymore enemies
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);

	//check if all enemies are dead only then resume to next wave with a countdown
	float interval = 4.f;
	GetWorld()->GetTimerManager().SetTimer(EnemiesDeadTimer, this, &AWaveManager::CheckIfAllEnemiesAreDead, interval, true);
}

void AWaveManager::Spawn()
{
	//spawn something
	UE_LOG(LogTemp, Warning, TEXT("Spawn enemy"));


	//check how many spawns have to happen
	int32 minimumEnemiesToSpawn = 1;
	size_t enemiesToSpawn = FMath::RandRange(minimumEnemiesToSpawn, MaxAmountOfEnemiesPerSpawn);

	for (size_t i{}; i < enemiesToSpawn; ++i)
	{
		//decide where to spawn it
		ABaseEnemySpawner* spawnPoint = GetRandomSpawnPoint();

		//decide what to spawn
		auto enemyType = GetRandomEnemyType();

		switch (enemyType)
		{
		case EEnemyType::Bomb:
			EnemyToSpawn = BombEnemy;
			break;
		case EEnemyType::Tank:
			EnemyToSpawn = TankEnemy;
			break;
		case EEnemyType::Assassin:
			EnemyToSpawn = AssassinEnemy;
			break;
		case EEnemyType::Caster:
			EnemyToSpawn = CastEnemy;
			break;
		}

		if (spawnPoint)
		{
			spawnPoint->SpawnEnemy(EnemyToSpawn, enemyType);
		}
	}
}

void AWaveManager::IncreaseWaveDifficulty()
{
	DecreaseSpawnTime();

	WaveTime += AddedWaveTime;
	//add more chance of difficult enemies

	ChangeEnemyPercentages();
	SetEnemyArray();
	//add more chance of spawning more enemies at once

}

EEnemyType const AWaveManager::GetRandomEnemyType()
{
	int32 randomIndex = FMath::RandRange(0, EnemiesForThisWave.Num() - 1);

	return EnemiesForThisWave[randomIndex];
}

ABaseEnemySpawner* AWaveManager::GetRandomSpawnPoint()
{
	int32 randomIndex = FMath::RandRange(0, Spawners.Num() - 1);
	return Cast<ABaseEnemySpawner>(Spawners[randomIndex]);
}

void AWaveManager::DecreaseSpawnTime()
{
	if (WaveNumber % AmountOfRoundsToDecreaseSpawnTimer == 0)
	{
		SpawnTime -= DecreasedSpawnTime;
	}

	if (SpawnTime < MinimumSpawnerTime)
	{
		SpawnTime = MinimumSpawnerTime;
	}
}

void AWaveManager::ChangeEnemyPercentages()
{
	//playtesting
	if (WaveNumber % AmountOfRoundsToChangePercentages == 0)
	{
		TankChancePercentage -= PercentageChange;
		BombChancePercentage -= PercentageChange;
		AssassinChancePercentage += PercentageChange;
		CasterChancePercentage += PercentageChange;
	}
}

void AWaveManager::IncreaseAmountOfSpawnsPerSpawn()
{
	if (WaveNumber % AmountOfRoundsToIncreaseAmountOfSpawns == 0)
	{
		++MaxAmountOfEnemiesPerSpawn;
	}

	//if (SpawnTime < MinimumSpawnerTime)
	//{
	//	SpawnTime = MinimumSpawnerTime;
	//}
}

void AWaveManager::SetEnemyArray()
{

	if (EnemiesForThisWave.Num() != 0)
	{
		EnemiesForThisWave.Reset();
	}


	if (TankChancePercentage + BombChancePercentage + AssassinChancePercentage + CasterChancePercentage != 100)
	{
		UE_LOG(LogTemp, Error, TEXT("Percentages do not sum up to a 100 ERROR"));
		return;
	}

	for (size_t i{}; i < TankChancePercentage; ++i)
	{
		EnemiesForThisWave.Add(EEnemyType::Tank);
	}
	for (size_t i{}; i < BombChancePercentage; ++i)
	{
		EnemiesForThisWave.Add(EEnemyType::Bomb);
	}
	for (size_t i{}; i < AssassinChancePercentage; ++i)
	{
		EnemiesForThisWave.Add(EEnemyType::Assassin);
	}
	for (size_t i{}; i < CasterChancePercentage; ++i)
	{
		EnemiesForThisWave.Add(EEnemyType::Caster);
	}

}

AAudioManager* AWaveManager::GetAudioManager() const
{
	//Get audio manager
	AAuroraGameModeBase* auroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (auroraGameMode)
		return auroraGameMode->GetAudioManager();

	UE_LOG(LogTemp, Warning, TEXT("Current gamemode is not the Aurora game mode."));
	return nullptr;
}