// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OptionsMenu.generated.h"

class UTextRenderComponent;
class AAudioManager;

UCLASS()
class AURORA_API AOptionsMenu : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOptionsMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetButtonsActive(bool bActive);

private:
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* MasterVolumeText;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* MusicVolumeText;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* SFXVolumeText;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* BackButton;

	AAudioManager* AudioManager;

	float CurrentMasterVolume;
	float CurrentMusicVolume;
	float CurrentSFXVolume;

	AAudioManager* GetAudioManager() const;
};
