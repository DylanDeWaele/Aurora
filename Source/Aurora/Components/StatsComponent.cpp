// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsComponent.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStatsComponent::AddWavesSurvived(int32 amount)
{
	PlayerStats.WavesSurvived += amount;
	PlayerStats.Score += amount;

	UE_LOG(LogTemp, Display, TEXT("Waves Survived: %i"), PlayerStats.EnemiesKilled);
}

void UStatsComponent::AddEnemiesKilled(int32 amount)
{
	PlayerStats.EnemiesKilled += amount;
	PlayerStats.Score += amount;

	UE_LOG(LogTemp, Display, TEXT("Enemies killed: %i"), PlayerStats.EnemiesKilled);
}

void UStatsComponent::AddEnemiesPurified(int32 amount)
{
	PlayerStats.EnemiesPurified += amount;
	PlayerStats.Score += amount;

	UE_LOG(LogTemp, Display, TEXT("Enemies purified: %i"), PlayerStats.EnemiesPurified);
}

void UStatsComponent::AddEnemiesTurned(int amount)
{
	PlayerStats.EnemiesTurned += amount;
	PlayerStats.Score += amount;

	UE_LOG(LogTemp, Display, TEXT("Enemies turned: %i"), PlayerStats.EnemiesTurned);
}

int32 UStatsComponent::GetScore() const
{
	return PlayerStats.Score;
}

int32 UStatsComponent::GetWavesSurvived() const
{
	return PlayerStats.WavesSurvived;
}

int32 UStatsComponent::GetEnemiesKilled() const
{
	return PlayerStats.EnemiesKilled;
}

int32 UStatsComponent::GetEnemiesPurified() const
{
	return PlayerStats.EnemiesPurified;
}

int32 UStatsComponent::GetEnemiesTurned() const
{
	return PlayerStats.EnemiesTurned;
}

const FPlayerStats& UStatsComponent::GetPlayerStats() const
{
	return PlayerStats;
}