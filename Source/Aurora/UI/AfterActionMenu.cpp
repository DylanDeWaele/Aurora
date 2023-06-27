// Fill out your copyright notice in the Description page of Project Settings.


#include "AfterActionMenu.h"
#include "Components/TextRenderComponent.h"
#include "BaseButton.h"

// Sets default values
AAfterActionMenu::AAfterActionMenu()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	//Create text components
	TimeSurvivedText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TimeSurvived"));
	EnemiesKilledText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("EnemiesKilled"));
	EnemiesPurifiedText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("EnemiesPurified"));
	EnemiesTurnedText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("EnemiesTurned"));
	TotalScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TotalScore"));

	BackButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("BackButton"));

	//Setup attachments
	TimeSurvivedText->SetupAttachment(root);
	EnemiesKilledText->SetupAttachment(root);
	EnemiesPurifiedText->SetupAttachment(root);
	EnemiesTurnedText->SetupAttachment(root);
	TotalScoreText->SetupAttachment(root);

	BackButton->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AAfterActionMenu::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAfterActionMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAfterActionMenu::SetStats(const FPlayerStats& PlayerStats)
{
	TimeSurvivedText->SetText(FText::FromString(FString::FromInt(PlayerStats.WavesSurvived)));
	EnemiesKilledText->SetText(FText::FromString(FString::FromInt(PlayerStats.EnemiesKilled)));
	EnemiesPurifiedText->SetText(FText::FromString(FString::FromInt(PlayerStats.EnemiesPurified)));
	EnemiesTurnedText->SetText(FText::FromString(FString::FromInt(PlayerStats.EnemiesTurned)));
	TotalScoreText->SetText(FText::FromString(FString::FromInt(PlayerStats.Score)));
}

void AAfterActionMenu::SetButtonsActive(bool bActive)
{
	if (bActive)
		Cast<ABaseButton>(BackButton->GetChildActor())->TurnCollisionOn();
	else
		Cast<ABaseButton>(BackButton->GetChildActor())->TurnCollisionOff();
}