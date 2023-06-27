// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Aurora/Components/StatsComponent.h"
#include "AfterActionMenu.generated.h"

class UTextRenderComponent;

UCLASS()
class AURORA_API AAfterActionMenu : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAfterActionMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStats(const FPlayerStats& PlayerStats);

	void SetButtonsActive(bool bActive);

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* TimeSurvivedText;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* EnemiesKilledText;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* EnemiesPurifiedText;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* EnemiesTurnedText;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* TotalScoreText;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* BackButton;
};
