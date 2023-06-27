// Fill out your copyright notice in the Description page of Project Settings.


#include "CasterEnemy.h"
#include "Aurora/Components/PurifiableComponent.h"
#include "Aurora/Components/TurnableComponent.h"
#include "Aurora/Actors/EnemyShootProjectile.h"
#include "Aurora/Actors/AudioManager.h"
#include "Aurora/Components/HealthComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"

ACasterEnemy::ACasterEnemy()
{
	
}

void ACasterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpecialAbility();

	if (ShootSound && ShootSound->IsPendingKill())
		ShootSound = nullptr;
}

void ACasterEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void ACasterEnemy::CheckIsTriggered()
{
	FVector distanceVector = GetActorLocation() - PlayerRef->GetActorLocation();

	if (distanceVector.Size() < FireDistance)
	{
		//set triggered
		IsTriggered = true;
		SetTriggerBlackBoardValue(true);

		//set focus
		Cast<AAIController>(GetController())->SetFocus(PlayerRef);

		//set timer
		GetWorld()->GetTimerManager().SetTimer(ShootTimer,this, &ACasterEnemy::ShootProjectile, FireRate, true, 0.f);
	}
}

void ACasterEnemy::ShootProjectile()
{
	GetMesh()->GetAnimInstance()->Montage_Play(ShootAnim);

	//Play casting sound
	ShootSound = AudioManager->PlaySound(EType::SFX, "Shooter_Charge",0, GetActorLocation());
}

void ACasterEnemy::SpecialAbility()
{
	//bad for performance but ALL other ways didn't work
	auto purify = Cast<UPurifiableComponent>(GetComponentByClass(UPurifiableComponent::StaticClass()));
	auto turn = Cast<UTurnableComponent>(GetComponentByClass(UTurnableComponent::StaticClass()));
	auto health = Cast<UHealthComponent>(GetComponentByClass(UHealthComponent::StaticClass()));

	if (!purify || !turn || !health)
	{
		return;
	}

	if (!IsTriggered && !purify->IsPurified() && !turn->IsTurned() && !health->IsDead())
	{
		CheckIsTriggered();
	}

	if (IsTriggered && (purify->IsPurified() || turn->IsTurned() || health->IsDead()))
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, ShootAnim);
		SetTriggerBlackBoardValue(false);
		GetWorld()->GetTimerManager().ClearTimer(ShootTimer);
		IsTriggered = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		if (ShootSound)
			ShootSound->Stop();
	}
}
