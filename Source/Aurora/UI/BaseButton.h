// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseButton.generated.h"

class AAudioManager;
class UTextRenderComponent;
class UBoxComponent;
class UPaperSprite;
class UPaperSpriteComponent;

UCLASS()
class AURORA_API ABaseButton : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ActivateButton(); //Pure virtual function

	virtual void DeactivateButton();

	void TurnCollisionOn();
	void TurnCollisionOff();

	bool IsActive() const;

protected:
	AAudioManager* AudioManager;

	bool bIsSelected = false;

private:
	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
		UPaperSprite* NonSelectedSprite;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
		UPaperSprite* SelectedSprite;

	UPROPERTY(EditAnywhere, Category = "Materials", meta = (AllowPrivateAccess = "true"))
		UPaperSpriteComponent* CurrentSprite;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* Collision;

	void CheckOverlapping();

	bool bIsActive = true;

	AAudioManager* GetAudioManager() const;
};
