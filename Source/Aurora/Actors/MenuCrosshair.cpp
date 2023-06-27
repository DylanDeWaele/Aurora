// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuCrosshair.h"
#include "Aurora/UI/BaseButton.h"

// Sets default values
AMenuCrosshair::AMenuCrosshair()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionChannel = ECC_Visibility;
}

// Called when the game starts or when spawned
void AMenuCrosshair::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMenuCrosshair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMenuCrosshair::PressButton()
{
	if (CollidedObject) 
	{
		//Check if its a button
		ABaseButton* button = Cast<ABaseButton>(CollidedObject);

		if (button && button->IsActive())
			button->ActivateButton();
	}
}

void AMenuCrosshair::ReleaseButton()
{
	if (CollidedObject)
	{
		//Check if its a button
		ABaseButton* button = Cast<ABaseButton>(CollidedObject);

		if (button && button->IsActive())
			button->DeactivateButton();
	}
}