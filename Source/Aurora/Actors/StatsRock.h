// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatsRock.generated.h"

class UTextRenderComponent;
class UStatsComponent;

UCLASS()
class AURORA_API AStatsRock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatsRock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComponent;

	//Text
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* TimeSurvivedTextComponent;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* EnemiesKilledTextComponent;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* EnemiesPurifiedTextComponent;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* EnemiesTurnedTextComponent;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* TotalScoreTextComponent;

	UStatsComponent* PlayerStats;

	void SetTexts();
};
