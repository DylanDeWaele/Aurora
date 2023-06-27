// Fill out your copyright notice in the Description page of Project Settings.


#include "BackButton.h"
#include "Aurora/UI/MainMenu.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABackButton::ABackButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABackButton::BeginPlay()
{
	Super::BeginPlay();
}

void ABackButton::ActivateButton()
{
	Super::ActivateButton();

	//Handle going back
	//Get the owner -> should be castable to a main menu and set the scoreboard visible
	AActor* owner = GetParentActor();

	if (owner)
	{
		AMainMenu* mainMenu = Cast<AMainMenu>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainMenu::StaticClass()));

		if (mainMenu)
			mainMenu->GoBack();
	}
}

void ABackButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}