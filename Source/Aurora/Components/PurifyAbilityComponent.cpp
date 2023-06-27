// Fill out your copyright notice in the Description page of Project Settings.


#include "PurifyAbilityComponent.h"
#include "DrawDebugHelpers.h"

#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Aurora/Components/PurifiableComponent.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "Aurora/Actors/PurifyBeam.h"
#include "Aurora/Actors/AudioManager.h"

#include "Components/AudioComponent.h"

UPurifyAbilityComponent::UPurifyAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Set the max ammo to 100, for the purify we will be working with a percentage
	MaxAmmo = 100;
}

void UPurifyAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	//Spawn an actor of type Purify beam in the world and disable it
	if (PurifyBeamClass && ShootSocket)
	{
		PurifyBeam = GetWorld()->SpawnActor<APurifyBeam>(PurifyBeamClass, ShootSocket->GetComponentTransform());
		PurifyBeam->EnableBeam(false);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("No Purify beam class set on the purify ability component or no shoot socket found."));

	//Get audio manager
	AudioManager = GetAudioManager();
}

void UPurifyAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsPurifying)
	{
		//Handle ammo
		CurrentAmmo -= DrainSpeed * DeltaTime;

		//Check if we ran out of ammo
		if (CheckNoAmmo())
		{
			DeactivateAbility();
		}
	}

	//locate and rotate the purify beam like the shoot socket
	if (PurifyBeam)
	{
		PurifyBeam->SetActorLocation(ShootSocket->GetComponentLocation());
		PurifyBeam->SetActorRotation(ShootSocket->GetComponentRotation());
	}

	if (ShootSound && ShootSound->IsPendingKill())
		ShootSound = nullptr;
}

void UPurifyAbilityComponent::ActivateAbility()
{
	bIsPurifying = true;

	ShootSound = AudioManager->PlaySound(EType::SFX, "Purify_Shoot", true);

	if (PurifyBeam)
		PurifyBeam->EnableBeam(true);
}

void UPurifyAbilityComponent::DeactivateAbility()
{
	bIsPurifying = false;

	if (ShootSound && AudioManager)
		AudioManager->StopSound(ShootSound);

	if (PurifyBeam)
		PurifyBeam->EnableBeam(false);
}

AAudioManager* UPurifyAbilityComponent::GetAudioManager() const
{
	//Get audio manager
	AAuroraGameModeBase* auroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (auroraGameMode)
		return auroraGameMode->GetAudioManager();

	UE_LOG(LogTemp, Warning, TEXT("Current gamemode is not the Aurora game mode."));
	return nullptr;
}