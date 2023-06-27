// Fill out your copyright notice in the Description page of Project Settings.


#include "DecrementVolumeButton.h"

ADecrementVolumeButton::ADecrementVolumeButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADecrementVolumeButton::BeginPlay()
{
	Super::BeginPlay();
}

void ADecrementVolumeButton::ActivateButton()
{
	//Super::ActivateButton();

	//Start decrementing value
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ADecrementVolumeButton::DecrementVolume, Interval, true, 0.f);
}

void ADecrementVolumeButton::DeactivateButton()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void ADecrementVolumeButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsSelected)
		DeactivateButton();
}

void ADecrementVolumeButton::DecrementVolume()
{
	AudioManager->DecrementVolume(VolumeType);

	AudioManager->PlaySound(EType::SFX, "Button_Click");
}