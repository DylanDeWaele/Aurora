// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlsButton.h"
#include "MainMenu.h"

AControlsButton::AControlsButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AControlsButton::BeginPlay()
{
	Super::BeginPlay();
}

void AControlsButton::ActivateButton()
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
			mainMenu->SetControlsMenuActive(true);
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Error trying to activate the controls menu."))
}

void AControlsButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}