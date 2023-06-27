// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Aurora/Components/StatsComponent.h"
#include "ScoreboardMenu.generated.h"

class UTextRenderComponent;

UCLASS()
class AURORA_API AScoreboardMenu : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AScoreboardMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateScores(const TArray<FPlayerStats>& Scores);

	void SetButtonsActive(bool bActive);

private:
	UPROPERTY(EditAnywhere, Category = "Ranks", meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* RankOne;
	UPROPERTY(EditAnywhere, Category = "Ranks", meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* RankTwo;
	UPROPERTY(EditAnywhere, Category = "Ranks", meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* RankThree;

	UPROPERTY(EditAnywhere, Category = "Ranks", meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* BackButton;

	bool bScoresLoaded = false;
};
