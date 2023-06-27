// Fill out your copyright notice in the Description page of Project Settings.


#include "NextButton.h"
#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANextButton::ANextButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ANextButton::BeginPlay()
{
	Super::BeginPlay();
}

void ANextButton::ActivateButton()
{
	Super::ActivateButton();

	//Get the owner -> should be castable to a main menu and set the scoreboard visible
	AActor* owner = GetParentActor();

	if (owner)
	{
		AMainMenu* mainMenu = Cast<AMainMenu>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainMenu::StaticClass()));

		if (mainMenu)
		{
			mainMenu->ShowNextControls();
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Error trying to activate the options menu."))
}

void ANextButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}