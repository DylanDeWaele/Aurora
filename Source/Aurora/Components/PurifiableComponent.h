// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/Components/BaseEffectComponent.h"
#include "PurifiableComponent.generated.h"

class UCapsuleComponent;
class UMaterialInterface;
class UNiagaraComponent;
class UTurnableComponent;
class AAudioManager;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AURORA_API UPurifiableComponent : public UBaseEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPurifiableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	bool IsPurified() const;
	UFUNCTION(BlueprintPure)
	bool IsBeingPurified() const;

	void StartPurification();
	void StopPurification();

private:
	UPROPERTY(EditAnywhere, Category = "Purification", meta = (AllowPrivateAccess = "true"))
		float PurifcationSpeed = 50.f; //The enemy will be purified after 5 seconds with this value

	UPROPERTY(EditAnywhere, Category = "Purification", meta = (AllowPrivateAccess = "true"))
		float DecaySpeed = 15.f;

	UPROPERTY(EditAnywhere, Category = "Purification", meta = (AllowPrivateAccess = "true"))
		int AmountOfMaterials = 3;

	float CurrentPurificationPercentage = 0; //If this reaches 100 then the actor has been purified

	UPROPERTY(EditInstanceOnly)
	bool bIsBeingPurified = false;

	bool bIsPurified = false;

	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UNiagaraComponent* ImpactParticles;
	UNiagaraComponent* PurifiedParticles;
	AAudioManager* AudioManager;

	void Purify();

	//Get the particle system components of the owner
	UNiagaraComponent* GetParticles(const FName& Tag) const;

	//This function will do a check on all the overlapping actors of the capsule component and if one of them is te beam we need to purify this actor
	void CheckIfBeingPurified();
	void HandlePurification(float DeltaTime);

	void SetIsPurifiedInBlackBoard();

	void InitializeDynamicMaterials();
	void UpdateMaterials();

	AAudioManager* GetAudioManager() const;
};
