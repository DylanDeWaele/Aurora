// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenu.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "Aurora/Actors/AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"
#include "BaseButton.h"

// Sets default values
AOptionsMenu::AOptionsMenu()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	MasterVolumeText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MasterVolume"));
	MusicVolumeText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MusicVolume"));
	SFXVolumeText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("SFXVolume"));

	BackButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("BackButton"));

	MasterVolumeText->SetupAttachment(root);
	MusicVolumeText->SetupAttachment(root);
	SFXVolumeText->SetupAttachment(root);

	BackButton->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AOptionsMenu::BeginPlay()
{
	Super::BeginPlay();

	AudioManager = GetAudioManager();
}

// Called every frame
void AOptionsMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MasterVolumeText->SetText(FText::AsNumber(AudioManager->GetMasterVolume() * 100));
	MusicVolumeText->SetText(FText::AsNumber(AudioManager->GetMusicVolume() * 100));
	SFXVolumeText->SetText(FText::AsNumber(AudioManager->GetSFXVolume() * 100));
}

void AOptionsMenu::SetButtonsActive(bool bActive)
{
	if (bActive)
		Cast<ABaseButton>(BackButton->GetChildActor())->TurnCollisionOn();
	else
		Cast<ABaseButton>(BackButton->GetChildActor())->TurnCollisionOff();
}

AAudioManager* AOptionsMenu::GetAudioManager() const
{
	//Get audio manager
	AAuroraGameModeBase* auroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (auroraGameMode)
		return auroraGameMode->GetAudioManager();

	UE_LOG(LogTemp, Warning, TEXT("Current gamemode is not the Aurora game mode."));
	return nullptr;
}