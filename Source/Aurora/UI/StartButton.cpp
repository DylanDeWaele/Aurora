// Fill out your copyright notice in the Description page of Project Settings.


#include "StartButton.h"
#include "Kismet/GameplayStatics.h"
#include "Aurora/Actors/AudioManager.h"
#include "Aurora/Characters/BaseVRCharacter.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "Components/AudioComponent.h"

// Sets default values
AStartButton::AStartButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AStartButton::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ABaseVRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AStartButton::ActivateButton()
{
	StartSound = AudioManager->PlaySound(EType::SFX, "Button_Start");
	bStartSoundPlayed = true;
	Player->FadeOut();

	//Fade out music
	Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->FadeOutMusic();
}

void AStartButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStartSoundPlayed)
	{
		if (!StartSound->IsPlaying())
			UGameplayStatics::OpenLevel(GetWorld(), "L_Main_01");
	}
}