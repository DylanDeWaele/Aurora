// Fill out your copyright notice in the Description page of Project Settings.


#include "AssassinEnemy.h"
#include "Aurora/Components/PurifiableComponent.h"
#include "Aurora/Components/TurnableComponent.h"
#include "Aurora/Actors/KillProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Aurora/Components/HealthComponent.h"


AAssassinEnemy::AAssassinEnemy()
{
	
	ProjectileDetectionBox = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileDetectionBox"));
	ProjectileDetectionBox->SetupAttachment(GetRootComponent());

	DashTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	UpdateTimeline.BindUFunction(this, FName{"TimelineFloatReturn"});	
	FinishedTimeline.BindUFunction(this, FName{"OnTimelineFinished"});
}

void AAssassinEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (fCurveDash)
	{
		DashTimeline->AddInterpFloat(fCurveDash, UpdateTimeline);
		DashTimeline->SetTimelineFinishedFunc(FinishedTimeline);

		DashTimeline->SetLooping(false);
	}
}

void AAssassinEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RemainingTime <= 0.0f)
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
	}
	else
	{
		RemainingTime -= DeltaTime;
	}
}

void AAssassinEnemy::CheckIsTriggered()
{
	if (ShouldDash())
	{
		//set timer
		RemainingTime = CooldownTime;

		IsTriggered = true;
		SetTriggerBlackBoardValue(true);
	
		//Set movementspeed
		GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
		GetCharacterMovement()->MaxAcceleration = DashAcceleration;
		
		//set dashdirection
		DashDirection = GetActorRightVector();

		////Start timer
		DashTimeline->PlayFromStart();

		//set orient
		GetCharacterMovement()->bOrientRotationToMovement = true;

		GetMesh()->GetAnimInstance()->Montage_Play(DashAnim);

		//set EndDelegate
		FOnMontageEnded OnMontageEnd;
		OnMontageEnd.BindUObject(this, &AAssassinEnemy::OnMontageEnded);

		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnd, DashAnim);
	}
}

bool AAssassinEnemy::ShouldDash()
{
	if(IsBeingEvaporated())
		return false;

	TArray<AActor*> overlappingActors;

	ProjectileDetectionBox->GetOverlappingActors(overlappingActors, AKillProjectile::StaticClass());

	if (overlappingActors.Num() != 0)
	{
		return true;
	}

	if (GetPurifiableComponent()->IsBeingPurified() || GetTurnableComponent()->IsBeingTurned())
	{
		return true;
	}

	return false;
}


void AAssassinEnemy::OnMontageEnded(UAnimMontage* Montage, bool bInteruppted)
{
	SetTriggerBlackBoardValue(false);
	IsTriggered = false;
}

//TIMELINE
void AAssassinEnemy::TimelineFloatReturn(float value)
{
	//add movement input
	AddMovementInput(DashDirection);
}

void AAssassinEnemy::OnTimelineFinished()
{
	//Set movementspeed
	GetCharacterMovement()->MaxWalkSpeed = 450.f;
	GetCharacterMovement()->MaxAcceleration = 2048.f;

	//set orient
	GetCharacterMovement()->bOrientRotationToMovement = true;
}
