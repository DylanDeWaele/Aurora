// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		int32 Score = 0;
	UPROPERTY(BlueprintReadWrite)
		int32 WavesSurvived = 0;
	UPROPERTY(BlueprintReadWrite)
		int32 EnemiesKilled = 0;
	UPROPERTY(BlueprintReadWrite)
		int32 EnemiesPurified = 0;
	UPROPERTY(BlueprintReadWrite)
		int32 EnemiesTurned = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AURORA_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatsComponent();

protected:
	virtual void BeginPlay() override;

public:
	void AddWavesSurvived(int32 amount);
	void AddEnemiesKilled(int32 amount);
	void AddEnemiesPurified(int32 amount);
	void AddEnemiesTurned(int32 amount);

	int32 GetScore() const;
	int32 GetWavesSurvived() const;
	int32 GetEnemiesKilled() const;
	int32 GetEnemiesPurified() const;
	int32 GetEnemiesTurned() const;

	const FPlayerStats& GetPlayerStats() const;

private:
	FPlayerStats PlayerStats;
};
