// Fill out your copyright notice in the Description page of Project Settings.


#include "KillAbilityComponent.h"

#include "Aurora/Actors/KillProjectile.h"
#include "Aurora/Characters/BaseVRCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"

UKillAbilityComponent::UKillAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxAmmo = 5.f;
}

void UKillAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UKillAbilityComponent::ActivateAbility()
{
	//The kill ability shoots a projectile that damages enemies on collision
	if (KillProjectileClass)
		ShootProjectile();
	else
		UE_LOG(LogTemp, Warning, TEXT("No kill projectile set on the kill ability component."));

	CurrentAmmo--;

	//Have to do the ammo check from the base ability
	CheckNoAmmo();
}

void UKillAbilityComponent::ShootProjectile() const
{
	//Instantiate a projectile from the shoot socket
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = Owner;
	spawnParams.Instigator = Owner->GetInstigator();

	AKillProjectile* spawnedProjectile = GetWorld()->SpawnActor<AKillProjectile>(KillProjectileClass, ShootSocket->GetComponentTransform(), spawnParams);

	//Fire it in the direction of the forward of the shoot socket
	if (spawnedProjectile)
		spawnedProjectile->FireInDirection(ShootSocket->GetForwardVector());
	else
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn a kill projectile."));
}