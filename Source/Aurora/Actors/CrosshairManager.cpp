// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairManager.h"
#include "Aurora/Actors/MenuCrosshair.h"

// Sets default values
ACrosshairManager::ACrosshairManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	StarDrawingCrosshair = CreateDefaultSubobject<UChildActorComponent>(TEXT("StarDrawingCrosshair"));
	KillAbilityCrosshair = CreateDefaultSubobject<UChildActorComponent>(TEXT("KillAbilityCrosshair"));
	MenuCrosshair = CreateDefaultSubobject<UChildActorComponent>(TEXT("MenuCrosshair"));

	StarDrawingCrosshair->SetupAttachment(root);
	KillAbilityCrosshair->SetupAttachment(root);
	MenuCrosshair->SetupAttachment(root);
}

void ACrosshairManager::SetCrosshair(ECrosshairType CrosshairType)
{
	CurrentCrosshair = CrosshairType;

	//Set the current crosshair as inactive
	TurnCrosshairOff();

	//switch crosshairs
	switch (CrosshairType)
	{
	case ECrosshairType::None:
		CurrentCrosshairActor = nullptr;
		break;
	case ECrosshairType::StarDrawing:
		CurrentCrosshairActor = StarDrawingCrosshair;
		break;
	case ECrosshairType::Kill:
		CurrentCrosshairActor = KillAbilityCrosshair;
		break;
	case ECrosshairType::Menu:
		CurrentCrosshairActor = MenuCrosshair;
		break;
	}

	//set current crosshair active
	TurnCrosshairOn();
}

ECrosshairType ACrosshairManager::GetCurrentCrosshair() const
{
	return CurrentCrosshair;
}

AMenuCrosshair* ACrosshairManager::GetMenuCrosshair() const
{
	AActor* childActor = MenuCrosshair->GetChildActor();

	if (childActor)
		return Cast<AMenuCrosshair>(childActor);

	return nullptr;
}

void ACrosshairManager::TurnCrosshairOn()
{
	if (IsValid(CurrentCrosshairActor))
		CurrentCrosshairActor->SetVisibility(true);
}

void ACrosshairManager::TurnCrosshairOff()
{
	if (IsValid(CurrentCrosshairActor))
		CurrentCrosshairActor->SetVisibility(false);
}
