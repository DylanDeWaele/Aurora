// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Aurora/UI/BaseButton.h"
#include "Aurora/UI/ScoreboardMenu.h"
#include "Aurora/UI/AfterActionMenu.h"
#include "Aurora/UI/ControlsMenu.h"
#include "Aurora/UI/OptionsMenu.h"
#include "Aurora/GameModes/AuroraGameInstance.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMainMenu::AMainMenu()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	//Buttons
	StartButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("StartButton"));
	ControlsButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("ControlsButton"));
	OptionsButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("OptionsButton"));
	ScoreboardButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("ScoreboardButton"));
	QuitButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("QuitButton"));

	//Menus
	ScoreboardMenu = CreateDefaultSubobject<UChildActorComponent>(TEXT("ScoreboardMenu"));
	OptionsMenu = CreateDefaultSubobject<UChildActorComponent>(TEXT("OptionsMenu"));
	AfterActionMenu = CreateDefaultSubobject<UChildActorComponent>(TEXT("AfterActionMenu"));
	ControlsMenu = CreateDefaultSubobject<UChildActorComponent>(TEXT("ControlsMenu"));

	//Setup attachments
	StartButton->SetupAttachment(root);
	ControlsButton->SetupAttachment(root);
	OptionsButton->SetupAttachment(root);
	ScoreboardButton->SetupAttachment(root);
	QuitButton->SetupAttachment(root);

	ScoreboardMenu->SetupAttachment(root);
	OptionsMenu->SetupAttachment(root);
	AfterActionMenu->SetupAttachment(root);
	ControlsMenu->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AMainMenu::BeginPlay()
{
	Super::BeginPlay();

	SetOptionsMenuActive(false);
	SetControlsMenuActive(false);
	SetScoreboardActive(false);

	UAuroraGameInstance* gameInstance = Cast<UAuroraGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (gameInstance->PlayerDied())
	{
		SetMainMenuActive(false);
		SetAfterActionMenuActive(true);

		//Set stats in after action menu
		Cast<AAfterActionMenu>(AfterActionMenu->GetChildActor())->SetStats(gameInstance->GetPlayerStats());

		gameInstance->SetPlayerDied(false);
	}
	else
	{
		SetMainMenuActive(true);
		SetAfterActionMenuActive(false);
	}

	ControlsMenuActor = GetControlsMenu();
}

// Called every frame
void AMainMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCameFromGame)
	{
		SetAfterActionMenuActive(true);
		bCameFromGame = false;
	}
}

void AMainMenu::SetScoreboardActive(bool bActive)
{
	//Turn on scoreboard
	ScoreboardMenu->SetVisibility(bActive);

	Cast<AScoreboardMenu>(ScoreboardMenu->GetChildActor())->SetButtonsActive(bActive);

	bOnScoreboardMenu = bActive;
}

void AMainMenu::SetOptionsMenuActive(bool bActive)
{
	//Turn on options
	OptionsMenu->SetVisibility(bActive);

	Cast<AOptionsMenu>(OptionsMenu->GetChildActor())->SetButtonsActive(bActive);

	bOnOptionsMenu = bActive;
}
void AMainMenu::SetAfterActionMenuActive(bool bActive)
{
	//Turn on scoreboard
	AfterActionMenu->SetVisibility(bActive);

	Cast<AAfterActionMenu>(AfterActionMenu->GetChildActor())->SetButtonsActive(bActive);

	bOnAfterActionMenu = bActive;
}

void AMainMenu::SetControlsMenuActive(bool bActive)
{
	//Turn on scoreboard
	ControlsMenu->SetVisibility(bActive);

	Cast<AControlsMenu>(ControlsMenu->GetChildActor())->SetButtonsActive(bActive);

	bOnControlsMenu = bActive;
}

void AMainMenu::GoBack()
{
	//Depending on we have multiple screens in each other, we need to change this function 
	SetMainMenuActive(true);

	//Check which menu to turn off
	if (bOnScoreboardMenu)
	{
		SetScoreboardActive(false);
	}
	else if (bOnAfterActionMenu)
	{
		SetAfterActionMenuActive(false);
	}
	else if (bOnControlsMenu)
	{
		if (ControlsMenuActor->IsOnFirstTutorial())
			SetControlsMenuActive(false);
		else
		{
			SetMainMenuActive(false);
			ControlsMenuActor->PreviousTutorial();
		}
	}
	else if (bOnOptionsMenu)
	{
		SetOptionsMenuActive(false);
	}
}

void AMainMenu::SetAfterActionStats(const FPlayerStats& PlayerStats)
{
	Cast<AAfterActionMenu>(AfterActionMenu->GetChildActor())->SetStats(PlayerStats);
}
void AMainMenu::SetCameFromGame()
{
	bCameFromGame = true;
}
void AMainMenu::SetMainMenuActive(bool bActive)
{
	//Turn on buttons
	SetButtonActive(bActive, StartButton);
	SetButtonActive(bActive, ControlsButton);
	SetButtonActive(bActive, OptionsButton);
	SetButtonActive(bActive, ScoreboardButton);
	SetButtonActive(bActive, QuitButton);
}

void AMainMenu::SetButtonActive(bool bActive, UChildActorComponent* Button)
{
	Button->SetVisibility(bActive);

	AActor* childActor = Button->GetChildActor();

	if (childActor)
	{
		ABaseButton* button = Cast<ABaseButton>(childActor);
		if (button)
		{
			//Turn the collision on or off
			if (bActive)
				button->TurnCollisionOn();
			else
				button->TurnCollisionOff();
		}
	}
}

void AMainMenu::ShowNextControls()
{
	AActor* childActor = ControlsMenu->GetChildActor();

	if (childActor)
	{
		AControlsMenu* controlMenu = Cast<AControlsMenu>(childActor);
		if (controlMenu)
		{
			controlMenu->NextTutorial();
		}
	}
}

void AMainMenu::ShowPreviousControls()
{
	AActor* childActor = ControlsMenu->GetChildActor();

	if (childActor)
	{
		AControlsMenu* controlMenu = Cast<AControlsMenu>(childActor);
		if (controlMenu)
		{
			controlMenu->PreviousTutorial();
		}
	}
}

AControlsMenu* AMainMenu::GetControlsMenu() const
{
	AActor* childActor = ControlsMenu->GetChildActor();

	if (childActor)
	{
		return Cast<AControlsMenu>(childActor);
	}

	return nullptr;
}