// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardButton.h"
#include "Aurora/UI/MainMenu.h"

// Sets default values
AScoreboardButton::AScoreboardButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AScoreboardButton::BeginPlay()
{
	Super::BeginPlay();
}

void AScoreboardButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AScoreboardButton::ActivateButton()
{
	Super::ActivateButton();

	//Get the owner -> should be castable to a main menu and set the scoreboard visible
	AActor* owner = GetParentActor();

	if(owner)
	{
		AMainMenu* mainMenu = Cast<AMainMenu>(owner);

		if (mainMenu) 
		{
			mainMenu->SetMainMenuActive(false);
			mainMenu->SetScoreboardActive(true);
		}
	}
}