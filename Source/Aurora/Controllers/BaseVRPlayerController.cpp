// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVRPlayerController.h"
#include "Aurora/Characters/BaseVRCharacter.h"
#include "Aurora/Components/AbilityManagerComponent.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ABaseVRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftTrigger", IE_Pressed, this, &ABaseVRPlayerController::LeftTriggerPressed);
	InputComponent->BindAction("LeftTrigger", IE_Released, this, &ABaseVRPlayerController::LeftTriggerReleased);

	InputComponent->BindAction("RightTrigger", IE_Pressed, this, &ABaseVRPlayerController::RightTriggerPressed);
	InputComponent->BindAction("RightTrigger", IE_Released, this, &ABaseVRPlayerController::RightTriggerReleased);

	InputComponent->BindAction("LeftHold", IE_Pressed, this, &ABaseVRPlayerController::LeftHoldPressed);
	InputComponent->BindAction("LeftHold", IE_Released, this, &ABaseVRPlayerController::LeftHoldReleased);

	InputComponent->BindAction("RightHold", IE_Pressed, this, &ABaseVRPlayerController::RightHoldPressed);
	InputComponent->BindAction("RightHold", IE_Released, this, &ABaseVRPlayerController::RightHoldReleased);

	InputComponent->BindAction("ActionButton", IE_Pressed, this, &ABaseVRPlayerController::ActionButtonPressed);
	InputComponent->BindAction("ActionButton", IE_Released, this, &ABaseVRPlayerController::ActionButtonReleased);
}

void ABaseVRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ABaseVRCharacter>(GetPawn());
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("no player pawn attached to controller"));
	}

	//Check if we are in menus or in game
		//If we are currently in the menu then we need to set our crosshair active, if not then don't
	AAuroraGameModeBase* gameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode)
		bInGame = gameMode->InGame();

}

void ABaseVRPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player)
	{
		if (bLeftDrawing)
			Player->Drawing(true);
		if (bRightDrawing)
			Player->Drawing(false);
	}
}

void ABaseVRPlayerController::ActivateAbility(bool bActivate)
{
	UActorComponent* abilityManager = Player->GetComponentByClass(UAbilityManagerComponent::StaticClass());
	if (abilityManager)
	{
		if (bActivate)
			//If we found it we can assume that we can immediately cast it to the ability manager and call the activate current ability method on it
			Cast<UAbilityManagerComponent>(abilityManager)->ActivateCurrentAbility();
		else
			Cast<UAbilityManagerComponent>(abilityManager)->DeactivateCurrentAbility();
	}
}

void ABaseVRPlayerController::StartDrawing(bool bLeftController)
{
	if (bLeftController)
	{
		UpdateLeftGrip(EGrip::Point, false);
		bLeftDrawing = true;
	}
	else
	{
		UpdateRightGrip(EGrip::Point, false);
		bRightDrawing = true;
	}
}

void ABaseVRPlayerController::UpdateLeftGrip(EGrip NewGrip, bool IsGrabOpen)
{
	if (NewGrip == EGrip::Open)
	{
		//you don't want to open the hand, if the open command came from the point gesture.
		if (LeftGripValue == EGrip::Grab && IsGrabOpen)
		{
			LeftGripValue = EGrip::Open;
		}
		else if (LeftGripValue != EGrip::Grab)
		{
			LeftGripValue = EGrip::Open;
		}
	}
	else if (NewGrip == EGrip::Grab)
	{
	//if you grab while pointing u will still see the pointing from the fist
		LeftGripValue = EGrip::Grab;
	}
	else if (NewGrip == EGrip::Point)
	{
		if (LeftGripValue != EGrip::Grab)
		{
			LeftGripValue = EGrip::Point;
		}
	}
}

void ABaseVRPlayerController::UpdateRightGrip(EGrip NewGrip, bool IsGrabOpen)
{
	if (NewGrip == EGrip::Open)
	{
		//you don't want to open the hand, if the open command came from the point gesture.
		if (RightGripValue == EGrip::Grab && IsGrabOpen)
		{
			RightGripValue = EGrip::Open;
		}
		else if (RightGripValue != EGrip::Grab)
		{
			RightGripValue = EGrip::Open;
		}
	}
	else if (NewGrip == EGrip::Grab)
	{
		//if you grab while pointing u will still see the pointing from the fist
		RightGripValue = EGrip::Grab;
	}
	else if (NewGrip == EGrip::Point)
	{
		if (RightGripValue != EGrip::Grab)
		{
			RightGripValue = EGrip::Point;
		}
	}
}

void ABaseVRPlayerController::StopDrawing(bool bLeftController)
{
	if (Player)
	{
		Player->StopDrawing(bLeftController);

		if (bLeftController)
		{
			UpdateLeftGrip(EGrip::Open, false);
			bLeftDrawing = false;
		}
		else
		{
			UpdateRightGrip(EGrip::Open, false);
			bRightDrawing = false;
		}
	}
}


void ABaseVRPlayerController::LeftTriggerPressed()
{
	if (Player)
	{
		if (bInGame)
		{
			//When pressed the only thing possible to happen is the star drawing
			//It can only be called when the left hand is free
			if (!Player->LeftHandHoldingStaff())
				StartDrawing(true);
			else  //We either shoot an ability if the player is holding the staff with the current hand
				ActivateAbility(true);
		}
		else Player->PressButton(true);
	}
}

void ABaseVRPlayerController::LeftTriggerReleased()
{
	if (Player)
	{
		if (bInGame)
		{
			if (bLeftDrawing)
				StopDrawing(true);

			//Also deactivate the current ability (if we are holding the staff)
			if (Player->LeftHandHoldingStaff())
				ActivateAbility(false);
		}
		else Player->ReleaseButton(true);
	}
}

void ABaseVRPlayerController::RightTriggerPressed()
{
	if (Player)
	{
		if (bInGame)
		{
			//When pressed the only thing possible to happen is the star drawing
			//It can only be called when the right hand is free
			if (!Player->RightHandHoldingStaff())
				StartDrawing(false);
			else //We either shoot an ability if the player is holding the staff with the current hand
				ActivateAbility(true);
		}
		else
			Player->PressButton(false);
	}
}

void ABaseVRPlayerController::RightTriggerReleased()
{
	if (Player)
	{
		if (bInGame)
		{
			if (bRightDrawing)
				StopDrawing(false);

			//Also deactivate the current ability (if we are holding the staff)
			if (Player->RightHandHoldingStaff())
				ActivateAbility(false);
		}
		else
			Player->ReleaseButton(false);
	}
}

void ABaseVRPlayerController::LeftHoldPressed()
{
	UpdateLeftGrip(EGrip::Grab, false);

	if (Player)
		Player->GrabActor(true);
}
void ABaseVRPlayerController::RightHoldPressed()
{
	UpdateRightGrip(EGrip::Grab, false);

	if (Player)
		Player->GrabActor(false);
}

void ABaseVRPlayerController::LeftHoldReleased()
{
	UpdateLeftGrip(EGrip::Open, true);

	if (Player)
		Player->ReleaseActor(true);
}
void ABaseVRPlayerController::RightHoldReleased()
{
	UpdateRightGrip(EGrip::Open, true);

	if (Player)
		Player->ReleaseActor(false);
}

void ABaseVRPlayerController::ActionButtonPressed()
{
	if (Player && bInGame)
		Player->ShowStats(true);
}

void ABaseVRPlayerController::ActionButtonReleased()
{
	if (Player && bInGame)
		Player->ShowStats(false);
}

EGrip ABaseVRPlayerController::GetLeftGripValue() const
{
	return LeftGripValue;
}

EGrip ABaseVRPlayerController::GetRightGripValue() const
{
	return RightGripValue;
}