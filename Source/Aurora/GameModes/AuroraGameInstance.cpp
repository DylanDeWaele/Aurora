// Fill out your copyright notice in the Description page of Project Settings.


#include "AuroraGameInstance.h"
#include "Aurora/Components//StatsComponent.h"
#include "Kismet/GameplayStatics.h"

void UAuroraGameInstance::SetPlayerDied(bool Dead)
{
	bPlayerDied = Dead;
}

bool UAuroraGameInstance::PlayerDied() const
{
	return bPlayerDied;
}

bool UAuroraGameInstance::ShouldSave() const
{
	return bShouldSave;
}

void UAuroraGameInstance::SavePlayerStats(const FPlayerStats& Stats)
{
	PlayerStats = Stats;

	Scores.Add(Stats);
	Scores.Sort([](const FPlayerStats& LHS, const FPlayerStats& RHS) {return LHS.Score > RHS.Score; });//Sort from highest score to lowest score

	bShouldSave = true;
}

void  UAuroraGameInstance::SetScores(const TArray<FPlayerStats>& SavedScores)
{
	Scores = SavedScores;
}

void  UAuroraGameInstance::SetShouldSave(bool ShouldGameSave)
{
	bShouldSave = ShouldGameSave;
}

const FPlayerStats& UAuroraGameInstance::GetPlayerStats() const
{
	return PlayerStats;
}

const TArray<FPlayerStats>& UAuroraGameInstance::GetScores() const
{
	return Scores;
}

void UAuroraGameInstance::SetGameLoaded(bool bIsGameLoaded)
{
	bGameLoaded = bIsGameLoaded;
}

bool UAuroraGameInstance::GameLoaded() const
{
	return bGameLoaded;
}