// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlsMenu.h"
#include "PaperSpriteComponent.h"
#include "BaseButton.h"

// Sets default values
AControlsMenu::AControlsMenu()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	ControlsScreen = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ControlScreen"));
	DrawingScreen = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("DrawingScreen"));
	EnemyScreen = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("EnemyScreen"));

	NextButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("NextButton"));
	BackButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("BackButton"));

	ControlsScreen->SetupAttachment(root);
	DrawingScreen->SetupAttachment(root);
	EnemyScreen->SetupAttachment(root);

	NextButton->SetupAttachment(root);
	BackButton->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AControlsMenu::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AControlsMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentScreen == 2)
	{
		NextButton->SetVisibility(false);
		Cast<ABaseButton>(NextButton->GetChildActor())->TurnCollisionOff();
	}
	else 
	{
		NextButton->SetVisibility(true);
		Cast<ABaseButton>(NextButton->GetChildActor())->TurnCollisionOn();
	}
}

void AControlsMenu::SetButtonsActive(bool bActive)
{
	if (bActive)
	{
		Cast<ABaseButton>(NextButton->GetChildActor())->TurnCollisionOn();
		Cast<ABaseButton>(BackButton->GetChildActor())->TurnCollisionOn();
	}
	else 
	{
		Cast<ABaseButton>(NextButton->GetChildActor())->TurnCollisionOff();
		Cast<ABaseButton>(BackButton->GetChildActor())->TurnCollisionOff();
	}
}

void AControlsMenu::NextTutorial()
{
	++CurrentScreen;

	if (CurrentScreen > 2)
		CurrentScreen = 2;

	//Reset
	ControlsScreen->SetVisibility(false);
	DrawingScreen->SetVisibility(false);
	EnemyScreen->SetVisibility(false);

	switch (CurrentScreen)
	{
	case 0:
		ControlsScreen->SetVisibility(true);
		break;
	case 1:
		DrawingScreen->SetVisibility(true);
		break;
	case 2:
		EnemyScreen->SetVisibility(true);
		break;
	}
}

void AControlsMenu::PreviousTutorial()
{
	--CurrentScreen;

	if (CurrentScreen < 0)
		CurrentScreen = 0;

	//Reset
	ControlsScreen->SetVisibility(false);
	DrawingScreen->SetVisibility(false);
	EnemyScreen->SetVisibility(false);

	switch (CurrentScreen)
	{
	case 0:
		ControlsScreen->SetVisibility(true);
		break;
	case 1:
		DrawingScreen->SetVisibility(true);
		break;
	case 2:
		EnemyScreen->SetVisibility(true);
		break;
	}
}

bool AControlsMenu::IsOnFirstTutorial() const
{
	return CurrentScreen == 0;
}

bool AControlsMenu::IsOnLastTutorial() const
{
	return CurrentScreen == 2;
}