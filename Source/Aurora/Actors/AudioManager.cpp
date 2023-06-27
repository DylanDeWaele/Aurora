// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundMix.h"

// Sets default values
AAudioManager::AAudioManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::SetBaseSoundMix(GetWorld(), SoundMix);
}

// Called every frame
void AAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAudioManager::StopSound(UAudioComponent* Sound)
{
	if (Sound)
	{
		Sound->Stop();
		Sound->DestroyComponent();
	}
	else

		UE_LOG(LogTemp, Warning, TEXT("Failed to stop sound."));
}

void AAudioManager::SetVolume(EType Type, float volume)
{
	switch (Type)
	{
	case EType::Master:
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, MasterSoundClass, volume);
		MasterVolume = volume;
		break;
	case EType::Music:
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, MusicSoundClass, volume);
		MusicVolume = volume;
		break;
	case EType::SFX:
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SFXSoundClass, volume);
		SFXVolume = volume;
		break;
	}
}

void AAudioManager::SetShouldSave(bool bSave)
{
	bShouldSave = bSave;
}

void AAudioManager::IncrementVolume(EType Type)
{
	switch (Type)
	{
	case EType::Master:
		MasterVolume += 0.01f;
		if (MasterVolume > 1.00f)
			MasterVolume = 1.f;
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, MasterSoundClass, MasterVolume);
		break;
	case EType::Music:
		MusicVolume += 0.01f;
		if (MusicVolume > 1.00f)
			MusicVolume = 1.f;
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, MusicSoundClass, MusicVolume);
		break;
	case EType::SFX:
		SFXVolume += 0.01f;
		if (SFXVolume > 1.00f)
			SFXVolume = 1.f;
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SFXSoundClass, SFXVolume);
		break;
	}

	bShouldSave = true;
}

void AAudioManager::DecrementVolume(EType Type)
{
	switch (Type)
	{
	case EType::Master:
		MasterVolume -= 0.01f;
		if (MasterVolume < 0.0f)
			MasterVolume = 0.f;
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, MasterSoundClass, MasterVolume);
		break;
	case EType::Music:
		MusicVolume -= 0.01f;
		if (MusicVolume < 0.0f)
			MusicVolume = 0.f;
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, MusicSoundClass, MusicVolume);
		break;
	case EType::SFX:
		SFXVolume -= 0.01f;
		if (SFXVolume < 0.0f)
			SFXVolume = 0.f;
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SFXSoundClass, SFXVolume);
		break;
	}

	bShouldSave = true;
}

float AAudioManager::GetMasterVolume() const
{
	return MasterVolume;
}

float AAudioManager::GetSFXVolume() const
{
	return SFXVolume;
}

float AAudioManager::GetMusicVolume() const
{
	return MusicVolume;
}

bool AAudioManager::ShouldSave() const
{
	return bShouldSave;
}

UAudioComponent* AAudioManager::PlaySound(EType SoundType, const FString& SoundName, float DelayTime, const FVector& Location)
{
	USoundCue** requestedSoundPtr = nullptr;
	USoundCue* requestedSound = nullptr;
	UAudioComponent* spawnedSound = nullptr;

	switch (SoundType)
	{
	case EType::SFX:
		//Look for the sound in the SFX map
		requestedSoundPtr = SoundEffects.Find(SoundName);
		break;
	case EType::Music:
		//Look for the sound in the song map
		requestedSoundPtr = Songs.Find(SoundName);
		break;
	}

	if (requestedSoundPtr)
		requestedSound = *requestedSoundPtr;

	//If no sound found, exit early
	if (!IsValid(requestedSound))
	{
		FString string = "Could not find sound: ";
		string.Append(SoundName);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, string);
		return spawnedSound;
	}

	//If we have a location, we should play at a location
	if (Location != FVector::ZeroVector)
		spawnedSound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), requestedSound, Location, FRotator::ZeroRotator, 1.f, 1.f, DelayTime);
	else
		spawnedSound = UGameplayStatics::SpawnSound2D(GetWorld(), requestedSound, 1.f, 1.f, DelayTime);

	return spawnedSound;
}