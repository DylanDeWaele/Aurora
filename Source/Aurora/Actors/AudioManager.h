// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioManager.generated.h"

class USoundCue;
class UAudioComponent;
class USoundMix;
class USoundClass;

UENUM(BlueprintType)
enum class EType : uint8
{
	SFX = 0,
	Music = 1,
	Master = 2
};

UCLASS()
class AURORA_API AAudioManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAudioManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Sound handling functions
	//This function will either play the sound normally, or with a given delay/at a location when given
	//The function also returns the sound in case we need to do something with it afterwards
	UAudioComponent* PlaySound(EType SoundType, const FString& SoundName, float DelayTime = 0.0f, const FVector& Location = { 0,0,0 });

	//Function to stop a sound
	void StopSound(UAudioComponent* Sound);

	UFUNCTION(BlueprintCallable)
		void SetVolume(EType Type, float volume);
	UFUNCTION(BlueprintCallable)
		void SetShouldSave(bool bSave);

	void IncrementVolume(EType Type);
	void DecrementVolume(EType Type);

	UFUNCTION(BlueprintCallable)
		float GetMasterVolume() const;
	UFUNCTION(BlueprintCallable)
		float GetSFXVolume() const;
	UFUNCTION(BlueprintCallable)
		float GetMusicVolume() const;
	UFUNCTION(BlueprintCallable)
		bool ShouldSave() const;

private:
	//Music
	UPROPERTY(EditAnywhere, Category = "Volume", meta = (AllowPrivateAccess = "true"))
		TMap<FString, USoundCue*> Songs;

	//Sound Effects
	UPROPERTY(EditAnywhere, Category = "Volume", meta = (AllowPrivateAccess = "true"))
		TMap<FString, USoundCue*> SoundEffects;

	//Volume control
	UPROPERTY(EditAnywhere, Category = "Volume", meta = (AllowPrivateAccess = "true"))
		USoundMix* SoundMix;
	UPROPERTY(EditAnywhere, Category = "Volume", meta = (AllowPrivateAccess = "true"))
		USoundClass* MasterSoundClass;
	UPROPERTY(EditAnywhere, Category = "Volume", meta = (AllowPrivateAccess = "true"))
		USoundClass* SFXSoundClass;
	UPROPERTY(EditAnywhere, Category = "Volume", meta = (AllowPrivateAccess = "true"))
		USoundClass* MusicSoundClass;

	float MasterVolume = 1.f;
	float SFXVolume = 1.f;
	float MusicVolume = 1.f;
	bool bShouldSave = false;
};
