// Fill out your copyright notice in the Description page of Project Settings.


#include "IncrementVolumeButton.h"

AIncrementVolumeButton::AIncrementVolumeButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AIncrementVolumeButton::BeginPlay()
{
	Super::BeginPlay();
}

void AIncrementVolumeButton::ActivateButton()
{
	//Super::ActivateButton();

	//Start decrementing value
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AIncrementVolumeButton::IncrementVolume, Interval, true, 0.f);
}

void AIncrementVolumeButton::DeactivateButton()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AIncrementVolumeButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsSelected)
		DeactivateButton();
}

void AIncrementVolumeButton::IncrementVolume()
{
	AudioManager->IncrementVolume(VolumeType);

	AudioManager->PlaySound(EType::SFX, "Button_Click");
}