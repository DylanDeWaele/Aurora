// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityManagerComponent.h"

//My includes
#include "Aurora/Components/BaseAbilityComponent.h"
#include "Aurora/Components/KillAbilityComponent.h"
#include "Aurora/Components/PurifyAbilityComponent.h"
#include "Aurora/Components/TurnAbilityComponent.h"
#include "Aurora/Actors/CrosshairManager.h"
#include "Aurora/Characters/BaseVRCharacter.h"
#include "Aurora/Actors/Staff.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
UAbilityManagerComponent::UAbilityManagerComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;
}

void UAbilityManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	//Set the player (owner)
	Owner = GetOwner();

	//Find and set the ability components
	KillAbilityComponent = GetKillAbilityComponent();
	PurifyAbilityComponent = GetPurifyAbilityComponent();
	TurnAbilityComponent = GetTurnAbilityComponent();

	//The default value can be changed in the editor so we set the active ability component in the begin play
	SetAbility(CurrentAbility);
}

void UAbilityManagerComponent::SetAbility(EAbility NewAbility)
{
	CurrentAbility = NewAbility;


	//Get the players crosshair & staff
	ACrosshairManager* crosshair = nullptr; //We only set the crosshair if the player is holding the staff
	AStaff* staff = Cast<AStaff>(UGameplayStatics::GetActorOfClass(GetWorld(), AStaff::StaticClass()));
	ABaseVRCharacter* player = Cast<ABaseVRCharacter>(Owner); //Check to see if the owner is a player or not -> the turret doesnt have a crosshair
	if (staff)
		crosshair = staff->GetCrosshair();

	switch (NewAbility)
	{
	case EAbility::None:
		CurrentAbilityComponent = nullptr;
		DisableCrosshair(crosshair, player);
		break;
	case EAbility::Kill:
		CurrentAbilityComponent = KillAbilityComponent;
		if (crosshair && (player->LeftHandHoldingStaff() || player->RightHandHoldingStaff()))
			crosshair->SetCrosshair(ECrosshairType::Kill);
		break;
	case EAbility::Purify:
		CurrentAbilityComponent = PurifyAbilityComponent;
		DisableCrosshair(crosshair, player);
		break;
	case EAbility::Turn:
		CurrentAbilityComponent = TurnAbilityComponent;
		DisableCrosshair(crosshair, player);
		break;
	}

	//Set staff particles + crystal materials
	if (staff)
		staff->SetAbility(NewAbility);

	//Fill up the ammo again of the ability
	if (CurrentAbilityComponent)
		CurrentAbilityComponent->RefillAmmo();
}

ECrosshairType UAbilityManagerComponent::GetCrosshairOfCurrentAbility() const
{
	switch (CurrentAbility)
	{
	case EAbility::None:
		return ECrosshairType::None;
		break;
	case EAbility::Kill:
		return ECrosshairType::Kill;
		break;
	case EAbility::Purify:
		break;
	case EAbility::Turn:
		break;
	}

	return ECrosshairType::None;
}

EAbility UAbilityManagerComponent::GetCurrentAbility() const
{
	return CurrentAbility;
}

float UAbilityManagerComponent::GetCurrentAbilityAmmoPercentage() const
{
	if (CurrentAbilityComponent)
		return CurrentAbilityComponent->GetCurrentAmmoPercentage();
	else 
		return 0.f;
}

void UAbilityManagerComponent::ActivateCurrentAbility()
{
	//If we have a current ability we should use it when this function is triggered
	if (CurrentAbilityComponent)
		CurrentAbilityComponent->ActivateAbility();
}

void UAbilityManagerComponent::DeactivateCurrentAbility()
{
	//Only the purify ability has the deactivate function declared
	if (CurrentAbilityComponent == PurifyAbilityComponent)
		CurrentAbilityComponent->DeactivateAbility();
}

UKillAbilityComponent* UAbilityManagerComponent::GetKillAbilityComponent() const
{
	UActorComponent* abilityComponent = FindAbilityComponent(EAbility::Kill);
	if (abilityComponent) //Check to see if we actually found one
	{
		//If we did we can assume we can immediately cast it to the kill ability and return it
		return Cast<UKillAbilityComponent>(abilityComponent);
	}

	UE_LOG(LogTemp, Warning, TEXT("No kill ability component found, this is okay for the turret."));
	return nullptr;
}

UPurifyAbilityComponent* UAbilityManagerComponent::GetPurifyAbilityComponent() const
{
	UActorComponent* abilityComponent = FindAbilityComponent(EAbility::Purify);
	if (abilityComponent) //Check to see if we actually found one
	{
		//If we did we can assume we can immediately cast it to the kill ability and return it
		return Cast<UPurifyAbilityComponent>(abilityComponent);
	}

	UE_LOG(LogTemp, Warning, TEXT("No purify ability component found, this is okay for the turret."));
	return nullptr;
}

UTurnAbilityComponent* UAbilityManagerComponent::GetTurnAbilityComponent() const
{
	UActorComponent* abilityComponent = FindAbilityComponent(EAbility::Turn);
	if (abilityComponent) //Check to see if we actually found one
	{
		//If we did we can assume we can immediately cast it to the kill ability and return it
		return Cast<UTurnAbilityComponent>(abilityComponent);
	}

	UE_LOG(LogTemp, Warning, TEXT("No turn ability component found, this is okay for the turret."));
	return nullptr;
}

UActorComponent* UAbilityManagerComponent::FindAbilityComponent(EAbility AbilityType) const
{
	UActorComponent* ability = nullptr;

	switch (AbilityType)
	{
	case EAbility::Kill:
		ability = Owner->GetComponentByClass(UKillAbilityComponent::StaticClass()); //this can be immediately casted to a kill ability without checking (assumed)
		break;
	case EAbility::Purify:
		ability = Owner->GetComponentByClass(UPurifyAbilityComponent::StaticClass()); //this can be immediately casted to a purify ability without checking (assumed)
		break;
	case EAbility::Turn:
		ability = Owner->GetComponentByClass(UTurnAbilityComponent::StaticClass()); //this can be immediately casted to a turn ability without checking (assumed)
		break;
	}

	return ability;
}

void UAbilityManagerComponent::DisableCrosshair(ACrosshairManager* Crosshair, ABaseVRCharacter* Player)
{
	if (Crosshair && (Player->LeftHandHoldingStaff() || Player->RightHandHoldingStaff()))
		Crosshair->SetCrosshair(ECrosshairType::None);
}