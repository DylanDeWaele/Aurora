// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Aurora/Components/StatsComponent.h"
#include "Aurora/GameModes/AuroraGameInstance.h"
#include "AuroraGameModeBase.generated.h"


class UHealthComponent;
class UChildActorComponent;
class AAudioManager;
class AMainMenu;
class ABaseVRCharacter;
class UAudioComponent;
class AWaveManager;

UCLASS()
class AURORA_API AAuroraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAuroraGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	AAudioManager* GetAudioManager() const;

	bool InGame() const;

	void FadeOutMusic();

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* AudioManager;

	//If this is false then we are in the menu level, if true then we are in game
	bool bInGame = false;
	bool bEndGame = false;
	bool bSavedGame = false;

	UAudioComponent* CurrentMusic;

	UAuroraGameInstance* GameInstance;
	ABaseVRCharacter* Player;
	AWaveManager* WaveManager;
};
