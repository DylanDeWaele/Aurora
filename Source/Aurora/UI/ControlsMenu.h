// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ControlsMenu.generated.h"

class UPaperSpriteComponent;

UCLASS()
class AURORA_API AControlsMenu : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AControlsMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NextTutorial();
	void PreviousTutorial();

	void SetButtonsActive(bool bActive);

	bool IsOnFirstTutorial() const;
	bool IsOnLastTutorial() const;


private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UPaperSpriteComponent* ControlsScreen;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UPaperSpriteComponent* DrawingScreen;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UPaperSpriteComponent* EnemyScreen;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* NextButton;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* BackButton;

	int CurrentScreen = 0;
};
