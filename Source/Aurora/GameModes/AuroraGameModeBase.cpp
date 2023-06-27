// Fill out your copyright notice in the Description page of Project Settings.


#include "AuroraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Aurora/Characters/BaseVRCharacter.h"
#include "Aurora/Components/HealthComponent.h"
#include "Aurora/Actors/AudioManager.h"
#include "Aurora/Actors/WaveManager.h"
#include "Aurora/UI/MainMenu.h"
#include "Components/AudioComponent.h"

AAuroraGameModeBase::AAuroraGameModeBase()
{
	//Create audio manager component
	AudioManager = CreateDefaultSubobject<UChildActorComponent>(TEXT("AudioManager"));
}

void AAuroraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ABaseVRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//Set in game
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "L_Main_01")
	{
		bInGame = true;
		CurrentMusic = GetAudioManager()->PlaySound(EType::Music, "Music_Game");

		WaveManager = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass()));
	}
	else
	{
		bInGame = false;
		CurrentMusic = GetAudioManager()->PlaySound(EType::Music, "Music_Menu");
	}

	//Fade in the music
	CurrentMusic->FadeIn(5.f);

	GameInstance = Cast<UAuroraGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AAuroraGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInGame)
	{
		if (Player->GetHealthComponent()->IsDead() && !bSavedGame)
		{
			bEndGame = true;
		}

		if (bEndGame)
		{
			//Save the game
			GameInstance->SetPlayerDied(true);
			GameInstance->SavePlayerStats(Player->GetStatsComponent()->GetPlayerStats());

			//Start fading out
			Player->FadeOut();

			//Play death sound
			GetAudioManager()->PlaySound(EType::SFX, "PlayerDeath");

			//Fade out music + heartbeat
			FadeOutMusic();
			UAudioComponent* heartbeat = Player->GetHeartbeat();
			if (heartbeat)
				heartbeat->FadeOut(4.f, 0.f);

			//Fade out drums
			if (WaveManager)
				WaveManager->FadeOutDrums();

			bEndGame = false;
			bSavedGame = true;
		}

		if (bSavedGame && Player->ScreenIsBlack())
			UGameplayStatics::OpenLevel(GetWorld(), "L_NorthernLights_02");
	}
}

void AAuroraGameModeBase::FadeOutMusic()
{
	CurrentMusic->FadeOut(4.f, 0.f);
}

AAudioManager* AAuroraGameModeBase::GetAudioManager() const
{
	AActor* childActor = AudioManager->GetChildActor();

	if (childActor)
	{
		AAudioManager* audioManager = Cast<AAudioManager>(childActor);;
		if (audioManager)
			return audioManager;
		else
			UE_LOG(LogTemp, Warning, TEXT("Actor assigned in the audio manager component of the gamemode is not an audio manager."));
	}

	UE_LOG(LogTemp, Warning, TEXT("No child actors assigned in the audio manager component."));
	return nullptr;
}

bool AAuroraGameModeBase::InGame() const
{
	return bInGame;
}