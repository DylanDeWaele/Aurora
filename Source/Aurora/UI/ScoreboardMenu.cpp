// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardMenu.h"
#include "Rank.h"
#include "Kismet/GameplayStatics.h"
#include "Aurora/GameModes/AuroraGameInstance.h"
#include "BaseButton.h"

// Sets default values
AScoreboardMenu::AScoreboardMenu()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	RankOne = CreateDefaultSubobject<UChildActorComponent>(TEXT("Rank1"));
	RankTwo = CreateDefaultSubobject<UChildActorComponent>(TEXT("Rank2"));
	RankThree = CreateDefaultSubobject<UChildActorComponent>(TEXT("Rank3"));

	BackButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("BackButton"));

	RankOne->SetupAttachment(root);
	RankTwo->SetupAttachment(root);
	RankThree->SetupAttachment(root);

	BackButton->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AScoreboardMenu::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AScoreboardMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bScoresLoaded)
	{
		UpdateScores(Cast<UAuroraGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetScores());
		bScoresLoaded = true;
	}
}

void AScoreboardMenu::UpdateScores(const TArray<FPlayerStats>& Scores)
{
	//I really don't like the way this is coded, but it will do for now
	for (int i = 0; i < Scores.Num(); i++)
	{
		if (i == 0)
			Cast<ARank>(RankOne->GetChildActor())->SetStats(Scores[0]);

		if (i == 1)
			Cast<ARank>(RankTwo->GetChildActor())->SetStats(Scores[1]);

		if (i == 2)
		{
			Cast<ARank>(RankThree->GetChildActor())->SetStats(Scores[2]);
			return;
		}
	}
}

void AScoreboardMenu::SetButtonsActive(bool bActive)
{
	if (bActive)
		Cast<ABaseButton>(BackButton->GetChildActor())->TurnCollisionOn();
	else
		Cast<ABaseButton>(BackButton->GetChildActor())->TurnCollisionOff();
}