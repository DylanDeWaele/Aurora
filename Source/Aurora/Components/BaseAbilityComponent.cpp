// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbilityComponent.h"

#include "Aurora/Actors/Staff.h"
#include "Aurora/Characters/BaseVRCharacter.h"
#include "Aurora/Components/AbilityManagerComponent.h"

#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UBaseAbilityComponent::UBaseAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UBaseAbilityComponent::RefillAmmo()
{
	CurrentAmmo = MaxAmmo;
}

void UBaseAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	//Get the owner
	Owner = GetOwner(); //We assume the owner is always valid

	//Get the staff
	Staff = UGameplayStatics::GetActorOfClass(GetWorld(), AStaff::StaticClass());

	//Find the shoot socket
	ShootSocket = FindShootSocket();
}

bool  UBaseAbilityComponent::CheckNoAmmo()
{
	if (CurrentAmmo <= 0.f)
	{
		UActorComponent* abilityManagerComponent = GetOwner()->GetComponentByClass(UAbilityManagerComponent::StaticClass());
		if (abilityManagerComponent)
		{
			//If we found it we can assume we can immediately cast it to a ability manager and set the ability
			Cast<UAbilityManagerComponent>(abilityManagerComponent)->SetAbility(EAbility::None);
			return true;
		}
	}

	return false;
}

UArrowComponent* UBaseAbilityComponent::FindShootSocket() const
{
	TArray<UActorComponent*> shootSockets{};

	//Shoot socket is on the staff if the owner of the ability manager is the player, else its on the owner itself
	if (Cast<ABaseVRCharacter>(Owner))
	{
		if (Staff)
			shootSockets = Staff->GetComponentsByTag(UArrowComponent::StaticClass(), "ShootSocket");
	}
	else
		shootSockets = Owner->GetComponentsByTag(UArrowComponent::StaticClass(), "ShootSocket");

	if (shootSockets.Num() > 0)
	{
		return Cast<UArrowComponent>(shootSockets[0]);
	}

	UE_LOG(LogTemp, Warning, TEXT("No shoot socket found."));
	return nullptr;
}

float UBaseAbilityComponent::GetCurrentAmmoPercentage() const
{
	return CurrentAmmo / MaxAmmo;
}
