// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseButton.h"
#include "Aurora/Actors/MenuCrosshair.h"
#include "Aurora/Actors/AudioManager.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"

#include "PaperSpriteComponent.h"

// Sets default values
ABaseButton::ABaseButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(root);

	CurrentSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("CurrentSprite"));
	CurrentSprite->SetupAttachment(root);
}

// Called when the game starts or when spawned
void ABaseButton::BeginPlay()
{
	Super::BeginPlay();

	AudioManager = GetAudioManager();
}

// Called every frame
void ABaseButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckOverlapping();

	//Set materials
	if (bIsSelected)
		CurrentSprite->SetSprite(SelectedSprite);
	else
		CurrentSprite->SetSprite(NonSelectedSprite);
}

void ABaseButton::ActivateButton()
{
	AudioManager->PlaySound(EType::SFX, "Button_Click");
}

void ABaseButton::DeactivateButton()
{
}

void ABaseButton::TurnCollisionOn()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bIsActive = true;
}

void ABaseButton::TurnCollisionOff()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsActive = false;
}

bool ABaseButton::IsActive() const
{
	return bIsActive;
}

void ABaseButton::CheckOverlapping()
{
	TArray<AActor*> overlappingActors{};
	GetOverlappingActors(overlappingActors);

	for (AActor* actor : overlappingActors)
	{
		if (Cast<AMenuCrosshair>(actor))
		{
			bIsSelected = true;
			return;
		}
	}

	bIsSelected = false;
}

AAudioManager* ABaseButton::GetAudioManager() const
{
	//Get audio manager
	AAuroraGameModeBase* auroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (auroraGameMode)
		return auroraGameMode->GetAudioManager();

	UE_LOG(LogTemp, Warning, TEXT("Current gamemode is not the Aurora game mode."));
	return nullptr;
}