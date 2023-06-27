// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsRock.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Aurora/Components/StatsComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AStatsRock::AStatsRock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	TimeSurvivedTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TimeSurvived"));
	EnemiesKilledTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("EnemiesKilled"));
	EnemiesPurifiedTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("EnemiesPurified"));
	EnemiesTurnedTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("EnemiesTurned"));
	TotalScoreTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TotalScore"));

	TimeSurvivedTextComponent->SetupAttachment(MeshComponent);
	EnemiesKilledTextComponent->SetupAttachment(MeshComponent);
	EnemiesPurifiedTextComponent->SetupAttachment(MeshComponent);
	EnemiesTurnedTextComponent->SetupAttachment(MeshComponent);
	TotalScoreTextComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AStatsRock::BeginPlay()
{
	Super::BeginPlay();

	//Find the stats component of the player
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (player)
	{
		UActorComponent* actorComponent = player->GetComponentByClass(UStatsComponent::StaticClass());

		if (actorComponent)
			PlayerStats = Cast<UStatsComponent>(actorComponent);
	}
}

// Called every frame
void AStatsRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Update the texts (we could make this more performant)
	if (PlayerStats)
	{
		SetTexts();
	}
}

void AStatsRock::SetTexts()
{
	TimeSurvivedTextComponent->SetText(FText::AsNumber(PlayerStats->GetWavesSurvived()));

	EnemiesKilledTextComponent->SetText(FText::AsNumber(PlayerStats->GetEnemiesKilled()));

	EnemiesPurifiedTextComponent->SetText(FText::AsNumber(PlayerStats->GetEnemiesPurified()));

	EnemiesTurnedTextComponent->SetText(FText::AsNumber(PlayerStats->GetEnemiesTurned()));

	TotalScoreTextComponent->SetText(FText::AsNumber(PlayerStats->GetScore()));
}