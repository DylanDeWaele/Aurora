// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVRCharacter.h"

#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/AudioComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

#include "Aurora/Actors/CrosshairManager.h"
#include "Aurora/Actors/MenuCrosshair.h"
#include "Aurora/Actors/StarField.h"
#include "Aurora/Actors/Star.h"
#include "Aurora/Actors/AudioManager.h"
#include "Aurora/Components/AbilityManagerComponent.h"
#include "Aurora/Components/KillAbilityComponent.h"
#include "Aurora/Components/PurifyAbilityComponent.h"
#include "Aurora/Components/TurnAbilityComponent.h"
#include "Aurora/Components/StatsComponent.h"
#include "Aurora/Components/PickupableComponent.h"
#include "Aurora/Components/HealthComponent.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "Aurora/Controllers/BaseVRPlayerController.h"

#include "DrawDebugHelpers.h"


// Sets default values
ABaseVRCharacter::ABaseVRCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	LeftController->SetupAttachment(VRRoot);
	LeftController->SetTrackingSource(EControllerHand::Left);
	LeftController->bDisplayDeviceModel = true;

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	RightController->SetupAttachment(VRRoot);
	RightController->SetTrackingSource(EControllerHand::Right);
	RightController->bDisplayDeviceModel = true;

	LeftGrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftGrabSphere"));
	LeftGrabSphere->SetupAttachment(LeftController);

	RightGrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightGrabSphere"));
	RightGrabSphere->SetupAttachment(RightController);

	//Abilities
	AbilityManager = CreateDefaultSubobject<UAbilityManagerComponent>(TEXT("AbilityManager"));
	KillAbiity = CreateDefaultSubobject<UKillAbilityComponent>(TEXT("KillAbility"));
	PurifyAbility = CreateDefaultSubobject<UPurifyAbilityComponent>(TEXT("PurifyAbility"));
	TurnAbility = CreateDefaultSubobject<UTurnAbilityComponent>(TEXT("TurnAbility"));

	//Stats
	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats"));

	//Health
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	//Post processing
	BlackScreenEffect = CreateDefaultSubobject<UPostProcessComponent>(TEXT("HealthEffect"));
	BlackScreenEffect->bUnbound = true;

	//Stats
	PlayerStatsActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("PlayerStats"));
	PlayerStatsActor->SetupAttachment(Camera);

	//vignette
	PlaneVignette = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneVignette"));
	PlaneVignette->SetupAttachment(Camera);
}

// Called when the game starts or when spawned
void ABaseVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AStaff::StaticClass());
	if (actor)
		Staff = Cast<AStaff>(actor);

	//Setup post processing
	InitializePostProcessingEffect();

	AudioManager = GetAudioManager();

	GameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	//Fade from black to game
	CurrentBlackScreenTransparency = 0;
	FadeIn();
}

// Called every frame
void ABaseVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCrosshairInitialized)
	{
		//If we are currently in the menu then we need to set our crosshair active, if not then don't
		SetupCrosshair();
		bCrosshairInitialized = true;
	}

	OffsetCameraToRoot();

	LineTraceHMDTowardsStarFields();

	//Fading
	HandleFading(DeltaTime);

	HandleHeartbeart();

	//set vignette
	HandleHealthVignette();

	if (CurrentFailSound && CurrentFailSound->IsPendingKill())
		CurrentFailSound = nullptr;

	if (HeartbeatSound && HeartbeatSound->IsPendingKill())
		HeartbeatSound = nullptr;
}

void ABaseVRCharacter::OffsetCameraToRoot()
{
	FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);
}

float ABaseVRCharacter::GetCurrentAmmoPercentage() const
{
	return AbilityManager->GetCurrentAbilityAmmoPercentage();
}

ACrosshairManager* ABaseVRCharacter::GetLeftCrosshair() const
{
	return GetCrosshairManager(true);
}

ACrosshairManager* ABaseVRCharacter::GetRightCrosshair() const
{
	return GetCrosshairManager(false);
}

UStatsComponent* ABaseVRCharacter::GetStatsComponent() const
{
	return StatsComponent;
}

UHealthComponent* ABaseVRCharacter::GetHealthComponent() const
{
	return HealthComponent;
}

UAudioComponent* ABaseVRCharacter::GetHeartbeat() const
{
	return HeartbeatSound;
}

bool ABaseVRCharacter::LeftHandHoldingStaff() const
{
	return bLeftHandHoldingStaff;
}

bool ABaseVRCharacter::RightHandHoldingStaff() const
{
	return bRightHandHoldingStaff;
}

void ABaseVRCharacter::LineTraceHMDTowardsStarFields()
{
	float reach = 10000.f;

	FHitResult hit;

	FVector StartPoint = Camera->GetComponentLocation();
	FVector EndPoint = StartPoint + Camera->GetForwardVector() * reach;

	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);

	bool hitSuccesfull = GetWorld()->LineTraceSingleByObjectType(hit, StartPoint, EndPoint, objectQueryParams);

	if (hitSuccesfull)
	{
		AStarField* hitStarField = Cast<AStarField>(hit.GetActor());
		HandleStarFields(hitStarField);
	}
	else
	{
		HandleStarFields(nullptr);
	}
}


void ABaseVRCharacter::HandleStarFields(AStarField* HitStarField)
{
	if (HitStarField)
	{
		//if you didn't hit anything first
		if (CurrentStarField == nullptr)
		{
			CurrentStarField = HitStarField;
			CurrentStarField->ShowPreview(true);

			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowMotionValue);
		}
		//check if its still the same star field
		else if (HitStarField == CurrentStarField)
		{
			//if its completed don't show it anymore
			if (CurrentStarField->IsPatternCorrect())
			{
				CurrentStarField->ShowPreview(false);
			}
			return;
		}
		//hit a different star field
		else
		{
			CurrentStarField->SetNewPatternOnCompleted();

			//hide previous starfield
			CurrentStarField->ShowPreview(false);
			CurrentStarField->DeActivateStars();
			CurrentStarField->ClearDrawnPattern();
			//show new starfield
			CurrentStarField = HitStarField;
			CurrentStarField->ShowPreview(true);
			CurrentStarField->ClearDrawnPattern();

			if (CurrentStarField->IsPatternCorrect())
			{
				CurrentStarField->SetIsPatternCorrect(false);
			}
		}
	}
	else
	{
		//if you didn't hit anything
		if (CurrentStarField)
		{
			CurrentStarField->ShowPreview(false);
			CurrentStarField->DeActivateStars();
			CurrentStarField->ClearDrawnPattern();

			if (CurrentStarField->IsPatternCorrect())
			{
				CurrentStarField->SetIsPatternCorrect(false);
			}

			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
			CurrentStarField = nullptr;
		}
	}
}


void ABaseVRCharacter::Drawing(bool bLeftController)
{
	if (bLeftController)
		GetLeftCrosshair()->SetCrosshair(ECrosshairType::StarDrawing);
	else
		GetRightCrosshair()->SetCrosshair(ECrosshairType::StarDrawing);

	AStar* starHit = IsStarHit(bLeftController);

	if (!starHit || !CurrentStarField)
		return;

	if (starHit->IsLit())
		return;

	CurrentStarField->NewStarHit(starHit->GetStarIndex());

	//check if the pattern is correct
	if (CurrentStarField->IsPatternCorrect())
	{
		//wait until rfx is done.

		CurrentStarField->OnCorrectPattern();
		ProduceAbility();

		//Play star completed sound
		if (AudioManager)
			AudioManager->PlaySound(EType::SFX, "Stars_Completed");
	}
}

AStar* ABaseVRCharacter::IsStarHit(bool bLeftController)
{
	float reach = 10000.f;
	FHitResult outHit;
	FVector controllerStart{};
	FVector controllerEnd{};

	if (bLeftController)
	{
		controllerStart = GetLeftCrosshair()->GetActorLocation();
		controllerEnd = controllerStart + GetLeftCrosshair()->GetActorForwardVector() * reach;
	}
	else
	{
		controllerStart = GetRightCrosshair()->GetActorLocation();
		controllerEnd = controllerStart + GetRightCrosshair()->GetActorForwardVector() * reach;
	}

	//switch to correct type of hand depending on player
	FVector startPoint = controllerStart;
	FVector endPoint = controllerEnd;

	bool bHit = GetWorld()->LineTraceSingleByChannel(outHit, startPoint, endPoint, ECollisionChannel::ECC_GameTraceChannel1);
	if (bHit)
	{
		AStar* hitStar = Cast<AStar>(outHit.GetActor());
		if (hitStar)
			return hitStar;
	}

	return nullptr;
}

void ABaseVRCharacter::StopDrawing(bool bLeftController)
{
	if (bLeftController)
		GetLeftCrosshair()->SetCrosshair(ECrosshairType::None);
	else
		GetRightCrosshair()->SetCrosshair(ECrosshairType::None);

	if (CurrentStarField)
	{
		//Play fail sound
		if (AudioManager && CurrentStarField->GetDrawnPattern().Num() > 0)
		{
			//Causes a crash
			if (CurrentFailSound)
				AudioManager->StopSound(CurrentFailSound);

			CurrentFailSound = AudioManager->PlaySound(EType::SFX, "Stars_Fail");
		}

		CurrentStarField->DeActivateStars();
		CurrentStarField->ClearDrawnPattern();
	}
}

void ABaseVRCharacter::GrabActor(bool bLeftController)
{
	AActor* actorNearHand = GetActorNearHand(bLeftController);

	if (IsValid(actorNearHand))
	{
		if (actorNearHand == Staff)
			GrabStaff(bLeftController);
		else
			GrabObject(bLeftController, actorNearHand);
	}
}

void ABaseVRCharacter::ReleaseActor(bool bLeftController)
{
	if (bLeftController)
	{
		if (IsValid(ActorInLeftHand))
		{
			if (ActorInLeftHand == Staff)
				ReleaseStaff(true);
			else
				ReleaseObject(true);
		}
	}
	else
	{
		if (IsValid(ActorInRightHand))
		{
			if (ActorInRightHand == Staff)
				ReleaseStaff(false);
			else
				ReleaseObject(false);
		}
	}
}

void ABaseVRCharacter::PressButton(bool bLeftController)
{
	ACrosshairManager* crosshair = nullptr;

	if (bLeftController)
		crosshair = GetLeftCrosshair();
	else
		crosshair = GetRightCrosshair();

	if (crosshair->GetCurrentCrosshair() == ECrosshairType::Menu)
		crosshair->GetMenuCrosshair()->PressButton();
}

void ABaseVRCharacter::ReleaseButton(bool bLeftController)
{
	ACrosshairManager* crosshair = nullptr;

	if (bLeftController)
		crosshair = GetLeftCrosshair();
	else
		crosshair = GetRightCrosshair();

	if (crosshair->GetCurrentCrosshair() == ECrosshairType::Menu)
		crosshair->GetMenuCrosshair()->ReleaseButton();
}

void ABaseVRCharacter::ShowStats(bool bShowStats)
{
	PlayerStatsActor->SetVisibility(bShowStats);
}

void ABaseVRCharacter::FadeIn()
{
	bFadingIn = true;
}

void ABaseVRCharacter::FadeOut()
{
	bFadingOut = true;
}

bool ABaseVRCharacter::ScreenIsBlack() const
{
	return CurrentBlackScreenTransparency <= 0;
}

ACrosshairManager* ABaseVRCharacter::GetCrosshairManager(bool bLeft) const
{
	TArray<UActorComponent*> crosshairActors{};

	if (bLeft)
		crosshairActors = GetComponentsByTag(UChildActorComponent::StaticClass(), "LeftCrosshair");
	else
		crosshairActors = GetComponentsByTag(UChildActorComponent::StaticClass(), "RightCrosshair");

	//Check if we found any
	if (crosshairActors.Num() > 0)
	{
		//Check if its has a child actor assigned
		AActor* crosshairChildActor = Cast<UChildActorComponent>(crosshairActors[0])->GetChildActor(); //We assume we can cast it to the child actor since we are getting the components by class
		if (IsValid(crosshairChildActor))
		{
			return Cast<ACrosshairManager>(crosshairChildActor);
		}

		UE_LOG(LogTemp, Warning, TEXT("No child actor set in the crosshair child actor on the player."));
		return nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("No crosshair actor found on the player."));
	return nullptr;
}

void ABaseVRCharacter::ProduceAbility()
{
	if (AbilityManager)
	{
		//Edge-case if we activated the turn ability we need to instantly spawn it
		if (CurrentStarField->GetAbility() == EAbility::Turn)
			TurnAbility->ActivateAbility();
		else
			AbilityManager->SetAbility(CurrentStarField->GetAbility());
	}
}

AActor* ABaseVRCharacter::GetActorNearHand(bool bLeftController)
{
	AActor* closestActor = nullptr;
	float nearestDistance = FLT_MAX;

	TArray<AActor*> actorsInGrabRange{};

	if (bLeftController)
		LeftGrabSphere->GetOverlappingActors(actorsInGrabRange);
	else
		RightGrabSphere->GetOverlappingActors(actorsInGrabRange);

	for (AActor* actor : actorsInGrabRange)
	{
		UPickupableComponent* pickupableComponent = Cast<UPickupableComponent>(actor->GetComponentByClass(UPickupableComponent::StaticClass()));
		if (pickupableComponent)
		{
			float currentDistance = 0;
			if (bLeftController)
				currentDistance = (actor->GetActorLocation() - LeftGrabSphere->GetComponentLocation()).SizeSquared();
			else
				currentDistance = (actor->GetActorLocation() - RightGrabSphere->GetComponentLocation()).SizeSquared();

			if (currentDistance < nearestDistance)
			{
				nearestDistance = currentDistance;
				closestActor = actor;
			}
		}
	}

	return closestActor;
}

void ABaseVRCharacter::GrabStaff(bool bLeftController)
{
	FStaffSocket* socket = nullptr;

	if (bLeftController)
	{
		//Find socket
		socket = GetStaffSocketInRange(LeftGrabSphere);

		if (socket) //If socket in range found
		{
			ActorInLeftHand = Staff;
			bLeftHandHoldingStaff = true;

			//Do a check to see if the socket was already used by the other controller, if it was set the other controllers variables correctly
			if (socket->Controller == RightController)
			{
				bRightHandHoldingStaff = false;
				ActorInRightHand = nullptr;
			}

			socket->Controller = LeftController;
			socket->bLeftController = true;

			//Always parent the new hand to the staff
			Cast<UPickupableComponent>(Staff->GetComponentByClass(UPickupableComponent::StaticClass()))->PickUp(LeftController);

			//Set the other controller back to visible (if this shouldn't be that case it will be handled by the handleSocketDisplay method in the staff)
			USceneComponent* mesh = RightController->GetChildComponent(2);
			if (mesh)
				mesh->SetVisibility(true);

			//If both hands are on the staff, do a re parent to the back socket
			if (bLeftHandHoldingStaff && bRightHandHoldingStaff)
				ReparentBackSocket();

			//Activate the crosshair
			Staff->GetCrosshair()->SetCrosshair(AbilityManager->GetCrosshairOfCurrentAbility());

			//Stop drawing with this hand if the play is in the process of doing that
			Cast<ABaseVRPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->StopDrawing(true);
		}
	}
	else
	{
		//Find socket
		socket = GetStaffSocketInRange(RightGrabSphere);

		if (socket)
		{
			if (socket->Component) //If socket in range found
			{
				ActorInRightHand = Staff;
				bRightHandHoldingStaff = true;

				// Do a check to see if the socket was already used by the other controller, if it was set the other controllers variables correctly
				if (socket->Controller == LeftController)
				{
					bLeftHandHoldingStaff = false;
					ActorInLeftHand = nullptr;
				}

				socket->Controller = RightController;
				socket->bLeftController = false;

				//Always parent the new hand to the staff
				Cast<UPickupableComponent>(Staff->GetComponentByClass(UPickupableComponent::StaticClass()))->PickUp(RightController);

				//Set the other controller back to visible (if this shouldn't be that case it will be handled by the handleSocketDisplay method in the staff)
				USceneComponent* mesh = LeftController->GetChildComponent(2);
				if (mesh)
					mesh->SetVisibility(true);

				//If both hands are on the staff, do a re parent to the back socket
				if (bLeftHandHoldingStaff && bRightHandHoldingStaff)
					ReparentBackSocket();

				//Activate the crosshair
				Staff->GetCrosshair()->SetCrosshair(AbilityManager->GetCrosshairOfCurrentAbility());

				//Stop drawing with this hand if the play is in the process of doing that
				Cast<ABaseVRPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->StopDrawing(false);
			}
		}
	}
}

void ABaseVRCharacter::GrabObject(bool bLeftController, AActor* ActorNearHand)
{
	UPickupableComponent* pickupableComponent = Cast<UPickupableComponent>(ActorNearHand->GetComponentByClass(UPickupableComponent::StaticClass()));
	if (pickupableComponent)
	{
		if (bLeftController)
		{
			pickupableComponent->PickUp(LeftController);
			ActorInLeftHand = ActorNearHand;
		}
		else
		{
			pickupableComponent->PickUp(RightController);
			ActorInRightHand = ActorNearHand;
		}
	}
}

void ABaseVRCharacter::ReleaseStaff(bool bLeftController)
{
	if (bLeftController)
	{
		ActorInLeftHand = nullptr;
		bLeftHandHoldingStaff = false;
		USceneComponent* mesh = LeftController->GetChildComponent(2);
		if (mesh)
			mesh->SetVisibility(true);

		FStaffSocket* socket = nullptr;

		//Check in which socket this hand is in and release it
		if (Staff->BackSocket.Controller == LeftController)
			socket = &Staff->BackSocket;
		else if (Staff->FrontSocket.Controller == LeftController)
			socket = &Staff->FrontSocket;

		if (socket)
		{
			if (socket->Controller == LeftController)
				socket->Controller = nullptr;
		}

		//If right hand is still holding the staff, just make sure it is the parent (perhaps re-parent)
		if (bRightHandHoldingStaff)
			Cast<UPickupableComponent>(Staff->GetComponentByClass(UPickupableComponent::StaticClass()))->PickUp(RightController); //Parent
		else //Else just release
			DropStaff();

	}
	else
	{
		ActorInRightHand = nullptr;
		bRightHandHoldingStaff = false;
		USceneComponent* mesh = RightController->GetChildComponent(2);
		if (mesh)
			mesh->SetVisibility(true);

		FStaffSocket* socket = nullptr;

		//Check in which socket this hand is in and release it
		if (Staff->BackSocket.Controller == RightController)
			socket = &Staff->BackSocket;
		else if (Staff->FrontSocket.Controller == RightController)
			socket = &Staff->FrontSocket;

		if (socket)
		{
			if (socket->Controller == RightController)
				socket->Controller = nullptr;
		}

		//If left hand is still holding the staff, just make sure it is the parent (perhaps re-parent)
		if (bLeftHandHoldingStaff)
			Cast<UPickupableComponent>(Staff->GetComponentByClass(UPickupableComponent::StaticClass()))->PickUp(LeftController); //Parent
		else //Else just release
			DropStaff();
	}
}

void ABaseVRCharacter::ReleaseObject(bool bLeftController)
{
	UPickupableComponent* pickupableComponent = nullptr;

	if (bLeftController)
	{
		pickupableComponent = Cast<UPickupableComponent>(ActorInLeftHand->GetComponentByClass(UPickupableComponent::StaticClass()));

		if (ActorInLeftHand != ActorInRightHand) //Extra check to make sure we don't mess up the drop
			pickupableComponent->Drop();

		ActorInLeftHand = nullptr;
	}
	else
	{
		pickupableComponent = Cast<UPickupableComponent>(ActorInRightHand->GetComponentByClass(UPickupableComponent::StaticClass()));

		if (ActorInRightHand != ActorInLeftHand) //Extra check to make sure we don't mess up the drop
			pickupableComponent->Drop();

		ActorInRightHand = nullptr;
	}
}

FStaffSocket* ABaseVRCharacter::GetStaffSocketInRange(USphereComponent* GrabSphere) const
{
	//Check components in this 
	TArray<UPrimitiveComponent*> components{};
	GrabSphere->GetOverlappingComponents(components);

	for (UPrimitiveComponent* component : components)
	{
		if (component == Staff->BackSocket.Component)
			return &Staff->BackSocket;
		else if (component == Staff->FrontSocket.Component)
			return &Staff->FrontSocket;
	}

	return nullptr;
}

void ABaseVRCharacter::ReparentBackSocket()
{
	if (Staff->BackSocket.Controller) //Only if one of the hands is actually on the back socket
		Cast<UPickupableComponent>(Staff->GetComponentByClass(UPickupableComponent::StaticClass()))->PickUp(Staff->BackSocket.Controller);
}

void ABaseVRCharacter::DropStaff()
{
	//Drop the staff
	Cast<UPickupableComponent>(Staff->GetComponentByClass(UPickupableComponent::StaticClass()))->Drop();

	//De activate the crosshair
	Staff->GetCrosshair()->SetCrosshair(ECrosshairType::None);

	//If we are purifying, disable the beam
	if (AbilityManager->GetCurrentAbility() == EAbility::Purify)
		AbilityManager->DeactivateCurrentAbility();
}

void ABaseVRCharacter::InitializePostProcessingEffect()
{
	//Set post processing effects
	BlackScreenEffectDynamicMaterial = UMaterialInstanceDynamic::Create(BlackScreenMaterial, this);
	BlackScreenEffectDynamicMaterial->SetScalarParameterValue("Transparency", 1.f);
	BlackScreenEffect->Settings.AddBlendable(BlackScreenEffectDynamicMaterial, 1.f);
}

void ABaseVRCharacter::SetupCrosshair()
{
	AAuroraGameModeBase* gameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode && !gameMode->InGame())
	{
		GetLeftCrosshair()->SetCrosshair(ECrosshairType::Menu);
		GetRightCrosshair()->SetCrosshair(ECrosshairType::Menu);
	}
}

AAudioManager* ABaseVRCharacter::GetAudioManager() const
{
	//Get audio manager
	AAuroraGameModeBase* auroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (auroraGameMode)
		return auroraGameMode->GetAudioManager();

	UE_LOG(LogTemp, Warning, TEXT("Current gamemode is not the Aurora game mode."));
	return nullptr;
}
void ABaseVRCharacter::HandleHeartbeart()
{
	const float healthPercentage = HealthComponent->GetHealthPercentage();
	int heartbeatIndex = 0;

	if (healthPercentage <= 0.1f)
		heartbeatIndex = 3;
	else if (healthPercentage <= 0.25f)
		heartbeatIndex = 2;
	else if (healthPercentage <= 0.5f)
		heartbeatIndex = 1;
	else if (healthPercentage <= 0.75f)
		heartbeatIndex = 0;
	else
		heartbeatIndex = -1;

	if (CurrentHeartbeatIndex != heartbeatIndex) //If there should be a change in heartbeat tempo
	{
		if (HeartbeatSound)
			HeartbeatSound->Stop();

		if (heartbeatIndex >= 0) //If -1, only stop the sound, don't play a new one
		{
			HeartbeatSound = AudioManager->PlaySound(EType::SFX, "Heartbeat");
			HeartbeatSound->SetIntParameter("HeartbeatIndex", heartbeatIndex);
		}

		CurrentHeartbeatIndex = heartbeatIndex;
	}	
}

void ABaseVRCharacter::HandleHealthVignette()
{
	const float healthPercentage = HealthComponent->GetHealthPercentage();

	//float x = FMath::Lerp(HealthVignetteValue, 0.f, healthPercentage);

	if (PlayerHealthVignette)
	{
		PlayerHealthVignetteDynamicMaterial = UMaterialInstanceDynamic::Create(PlayerHealthVignette, PlaneVignette);
		PlayerHealthVignetteDynamicMaterial->SetScalarParameterValue("Mask Position", healthPercentage);

		PlaneVignette->SetMaterial(0, PlayerHealthVignetteDynamicMaterial);
	}
}

void ABaseVRCharacter::HandleFading(float DeltaTime)
{
	if (bFadingIn)
	{
		CurrentBlackScreenTransparency += FadeSpeed * DeltaTime;

		if (CurrentBlackScreenTransparency >= 1)
			bFadingIn = false;
	}
	else if (bFadingOut)
	{
		CurrentBlackScreenTransparency -= FadeSpeed * DeltaTime;

		if (CurrentBlackScreenTransparency <= 0)
			bFadingOut = false;
	}

	BlackScreenEffectDynamicMaterial->SetScalarParameterValue("Transparency", CurrentBlackScreenTransparency);
}