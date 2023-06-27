// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Aurora/Components/StatsComponent.h"
#include "AuroraGameInstance.generated.h"

/**
 *
 */
UCLASS()
class AURORA_API UAuroraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void SetPlayerDied(bool Dead);
	bool PlayerDied() const;

	void SavePlayerStats(const FPlayerStats& Stats);
	const FPlayerStats& GetPlayerStats() const;

	UFUNCTION(BlueprintCallable)
		void SetScores(const TArray<FPlayerStats>& SavedScores);
	UFUNCTION(BlueprintCallable)
		const TArray<FPlayerStats>& GetScores() const;

	UFUNCTION(BlueprintCallable)
		void SetShouldSave(bool bGameShouldSave);
	UFUNCTION(BlueprintCallable)
		bool ShouldSave() const;

	UFUNCTION(BlueprintCallable)
		void SetGameLoaded(bool bIsGameLoaded);
	UFUNCTION(BlueprintCallable)
		bool GameLoaded() const;

private:
	bool bPlayerDied = false;
	bool bShouldSave = false;
	bool bGameLoaded = false;

	TArray<FPlayerStats> Scores;
	FPlayerStats PlayerStats;

};
