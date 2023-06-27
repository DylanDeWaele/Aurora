// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsButton.h"
#include "Aurora/UI/MainMenu.h"

// Sets default values
AOptionsButton::AOptionsButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AOptionsButton::BeginPlay()
{
	Super::BeginPlay();
}

void AOptionsButton::ActivateButton()
{
	Super::ActivateButton();

	//Get the owner -> should be castable to a main menu and set the scoreboard visible
	AActor* owner = GetParentActor();

	if (owner)
	{
		AMainMenu* mainMenu = Cast<AMainMenu>(owner);

		if (mainMenu)
		{
			mainMenu->SetMainMenuActive(false);
			mainMenu->SetOptionsMenuActive(true);
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Error trying to activate the options menu."))
}

void AOptionsButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}