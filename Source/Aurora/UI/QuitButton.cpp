// Fill out your copyright notice in the Description page of Project Settings.


#include "QuitButton.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AQuitButton::AQuitButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AQuitButton::BeginPlay()
{
	Super::BeginPlay();
}

void AQuitButton::ActivateButton()
{
	Super::ActivateButton();

	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void AQuitButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
