// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aurora/Components/BaseEffectComponent.h"
#include "TurnableComponent.generated.h"

class UMaterialInstanceDynamic;
class UPurifiableComponent;
class UNiagaraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AURORA_API UTurnableComponent : public UBaseEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTurnableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
		bool IsBeingTurned() const;

	UFUNCTION(BlueprintPure)
		bool IsTurned() const;

private:
	UPROPERTY(EditAnywhere, Category = "Turning", meta = (AllowPrivateAccess = "true"))
		float TurnSpeed = 50.f; //THe enemy will be turned after 5 seconds with this value

	UPROPERTY(EditAnywhere, Category = "Turning", meta = (AllowPrivateAccess = "true"))
		float DecaySpeed = 15.f;

	UPROPERTY(EditAnywhere, Category = "Turning", meta = (AllowPrivateAccess = "true"))
		int AmountOfMaterials = 3;

	float CurrentTurnPercentage = 0; //if this reaches 100 then the actor has been turned

	UPROPERTY(EditAnywhere)
		bool bIsBeingTurned = false;

	bool bIsTurned = false;
	
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UNiagaraComponent* TurnedParticles;

	UNiagaraComponent* GetTurnedParticles() const;

	void CheckIfBeingTurned();

	void HandleTurning(float DeltaTime);

	void Turn();

	void SetIsTurnedInBlackBoard();

	void InitializeDynamicMaterials();
	void UpdateMaterials();
};