// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURORA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void DecreaseHealth(int amount);
	void IncreaseHealth(int amount);

	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	int32 GetCurrentHealth() const;
	float GetHealthPercentage() const;

private:
	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
		int32 MaxHealth = 100;

	int32 CurrentHealth;
	bool bIsDead = false;

	void SetIsDeadInBlackBoard();
};
