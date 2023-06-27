// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Aurora/Components/StatsComponent.h"
#include "MainMenu.generated.h"

class AControlsMenu;

UCLASS()
class AURORA_API AMainMenu : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMainMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMainMenuActive(bool bActive);
	void SetScoreboardActive(bool bActive);
	void SetOptionsMenuActive(bool bActive);
	void SetControlsMenuActive(bool bActive);
	void SetAfterActionMenuActive(bool bActive);

	void GoBack();

	void SetCameFromGame();
	void SetAfterActionStats(const FPlayerStats& PlayerStats);

	void ShowNextControls();
	void ShowPreviousControls();

private:
	//Buttons
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* StartButton;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* OptionsButton;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ScoreboardButton;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* QuitButton;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ControlsButton;

	//Other menus
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ScoreboardMenu; //Another menu
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* OptionsMenu;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ControlsMenu; //Also has a control scheme

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* AfterActionMenu; //Menu that gets displayed when the game ends, showing the players score

	//Menu variables
	bool bOnOptionsMenu = false;
	bool bOnScoreboardMenu = false;
	bool bOnAfterActionMenu = false;
	bool bOnControlsMenu = false;
	bool bCameFromGame = false;

	AControlsMenu* ControlsMenuActor = nullptr;

	AControlsMenu* GetControlsMenu() const;

	void SetButtonActive(bool bActive, UChildActorComponent* Button);
};
