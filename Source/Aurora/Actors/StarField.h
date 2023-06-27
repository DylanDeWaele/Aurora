// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Enums/Ability.h"
#include "StarField.generated.h"

class UBoxComponent;
class UChildActorComponent;
class USplineComponent;
class USplineMeshComponent;
class AAudioManager;
class UAudioComponent;
class UNiagaraSystem;

UCLASS()
class AURORA_API AStarField : public AActor
{
	GENERATED_BODY()
	
public:	
	AStarField();	

	virtual void Tick(float DeltaTime) override;

	//STARS
	void DeActivateStars();
	void NewStarHit(int32 StarIndex);
	void SetCollisionOnPreviewStars(bool CollsionOn);

	//PATTERN
	void ShowPreview(bool Enable);
	bool IsPatternCorrect() const;
	void ClearDrawnPattern();
	void SetNewPatternOnCompleted();
	void OnCorrectPattern();
	bool IsCompleted() const {return IsPatternCompleted;}
	void SetIsPatternCorrect(bool Enable) {IsPatternCompleted = Enable;}

	//ABILITY
	EAbility GetAbility() const { return Ability; }

	const TArray<int32>& GetDrawnPattern() const;

protected:

	virtual void BeginPlay() override;

private:
	//GENERAL
	bool IsActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
		EAbility Ability = EAbility::None;

	//STARS
	UPROPERTY(EditAnywhere, Category = "Stars", meta = (AllowPrivateAccess = "true"))
		float DrawOffset = 30.f;

	//SIZES
	const int32 AmountOfStars = 10;

	static int32 AmountOfCreatedStars;

	UPROPERTY(EditAnywhere, Category = "Pattern", meta = (AllowPrivateAccess ="true"))
		int32 MinimumPatternSize = 4;

	UPROPERTY(EditAnywhere, Category = "Pattern", meta = (AllowPrivateAccess = "true"))
		int32 MaximumPatternSize = AmountOfStars;

	//COMPONENTS
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USplineComponent* DrawnSpline;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USplineComponent* PreviewSpline;

	UPROPERTY(VisibleAnywhere, Category = "Stars", meta = (AllowPrivateAccess = "true"))
		TArray<UChildActorComponent*> Stars;

	//SPLINE_MESHES
	UPROPERTY(EditDefaultsOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* DrawnMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Spline", meta = (AllowPrivateAccess= "true"))
		UStaticMesh* PreviewMesh;

	TArray<USplineMeshComponent*> DrawnSplineMeshes;
	TArray<USplineMeshComponent*> PreviewSplineMeshes;

	//Particle system
	UPROPERTY(EditDefaultsOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* StarCompletionParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* DrawnSplineMeshParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* PreviewSplineMeshParticles;

	//divider that will make the lenght of the particle relative
	UPROPERTY(EditDefaultsOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		float PreviewSplineDivider = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		float DrawSplineDivider = 100.f;

	//METHODS
	//-setup-
	void SetupStars();
	void SetupCollision();
	void SetIndices();
	void HideAllStars();

	//-splines-
	void ConstructSplineMesh(int32 Index, UStaticMesh* const SplineMesh, USplineComponent* const Spline, TArray<USplineMeshComponent*>& SplineMeshList, bool IsPreviewSpline);
	void DestructSplineMesh(USplineComponent* const Spline, TArray<USplineMeshComponent*>& SplineMeshes);
	void TogglePreviewSpline(bool Enable);
	void SetBeginPointOfPreviewSpline();
	void ResetBeginPointOfPreviewSpline();
	void MakeNewPreviewSpline();

	//-pattern-
	void MakeNewPreviewPattern();
	void ResetPreviousPattern();
	void CreateRandomPattern();

	TArray<int32> PreviewPattern;
	TArray<int32> DrawnPattern;

	//whether or not the player completed this pattern
	bool IsPatternCompleted = false;

	AAudioManager* AudioManager;
	UAudioComponent* CurrentHoverSound = nullptr;

	AAudioManager* GetAudioManager() const;
};
