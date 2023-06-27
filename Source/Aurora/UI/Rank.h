// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Aurora/Components/StatsComponent.h"
#include "Rank.generated.h"

class UTextRenderComponent;

UCLASS()
class AURORA_API ARank : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStats(const FPlayerStats& Stats);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		USceneComponent* Root;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* TimeSurvived;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* EnemiesKilled;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* EnemiesPurified;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* EnemiesTurned;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* TotalScore;
};
