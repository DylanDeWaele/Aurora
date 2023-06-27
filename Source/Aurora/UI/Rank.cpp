// Fill out your copyright notice in the Description page of Project Settings.


#include "Rank.h"
#include "Components/TextRenderComponent.h"


// Sets default values
ARank::ARank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	TimeSurvived = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TimeSurvived"));
	EnemiesKilled = CreateDefaultSubobject<UTextRenderComponent>(TEXT("EnemiesKilled"));;
	EnemiesPurified = CreateDefaultSubobject<UTextRenderComponent>(TEXT("EnemiesPurified"));;
	EnemiesTurned = CreateDefaultSubobject<UTextRenderComponent>(TEXT("EnemiesTurned"));;
	TotalScore = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TotalScore"));;

	TimeSurvived->SetupAttachment(Root);
	EnemiesKilled->SetupAttachment(Root);
	EnemiesPurified->SetupAttachment(Root);
	EnemiesTurned->SetupAttachment(Root);
	TotalScore->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ARank::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARank::SetStats(const FPlayerStats& Stats)
{
	TimeSurvived->SetText(FText::FromString(FString::FromInt(Stats.WavesSurvived)));
	EnemiesKilled->SetText(FText::FromString(FString::FromInt(Stats.EnemiesKilled)));
	EnemiesPurified->SetText(FText::FromString(FString::FromInt(Stats.EnemiesPurified)));
	EnemiesTurned->SetText(FText::FromString(FString::FromInt(Stats.EnemiesTurned)));
	TotalScore->SetText(FText::FromString(FString::FromInt(Stats.Score)));
}
