// Fill out your copyright notice in the Description page of Project Settings.


#include "TankEnemy.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Aurora/Actors/AudioManager.h"
#include "Aurora/Components/TurnableComponent.h"
#include "Aurora/Components/PurifiableComponent.h"
#include "Aurora/Components/HealthComponent.h"

ATankEnemy::ATankEnemy()
{
}

void ATankEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//bad for performance but ALL other ways didn't work
	auto purify = Cast<UPurifiableComponent>(GetComponentByClass(UPurifiableComponent::StaticClass()));
	auto turn = Cast<UTurnableComponent>(GetComponentByClass(UTurnableComponent::StaticClass()));
	auto health = Cast<UHealthComponent>(GetComponentByClass(UHealthComponent::StaticClass()));

	if (!purify || !turn || !health)
	{
		return;
	}

	if(!IsTriggered && !purify->IsPurified() && !turn->IsTurned() && !health->IsDead())
		CheckIsTriggered();

	if (IsTriggered && (purify->IsPurified() || turn->IsTurned() || health->IsDead()))
	{
		IsTriggered = false;
		//set speed back
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
}

void ATankEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ATankEnemy::CheckIsTriggered()
{
	FVector distanceVector = GetActorLocation() - PlayerRef->GetActorLocation();

	if (distanceVector.Size() < EnrageDistance)
	{
		IsTriggered = true;
		SetTriggerBlackBoardValue(true);

		//increase speed
		GetCharacterMovement()->MaxWalkSpeed = EnragedSpeed;

		//play animation
		GetMesh()->GetAnimInstance()->Montage_Play(RoarAnimation);

		//set EndDelegate
		FOnMontageEnded OnMontageEnd;
		OnMontageEnd.BindUObject(this, &ATankEnemy::OnMontageEnded);

		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnd, RoarAnimation);

		//Play roar
		AudioManager->PlaySound(EType::SFX, "Tank_Growl",0, GetActorLocation());
	}
}

void ATankEnemy::OnMontageEnded(UAnimMontage* Montage, bool bInteruppted)
{
	SetTriggerBlackBoardValue(false);
}
