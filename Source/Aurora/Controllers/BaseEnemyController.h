// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyController.generated.h"

class UPurifiableComponent;
class UTurnableComponent;

UCLASS()
class AURORA_API ABaseEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	ABaseEnemyController();

	virtual void Tick(float DeltaTime) override;

	const FVector& GetCurrentMovementLocation() const;
	void IncrementLocationIndex();

	UFUNCTION(BlueprintNativeEvent, Category = "PathFinding")
		void InitAndStartMoving(const TArray<FVector>& locations, EEnemyType EnemyType);

	bool IsNextLocationValid() const;

	bool GetHasReachedLastPoint() const {return HasReachedLastPoint;}

	void SetHasReachedLastPoint(bool HasReached) {HasReachedLastPoint = HasReached;}
	
	void SetPlayerLocationInBlackBoard();

	void SetCanMoveInBlackboard();

protected:
	virtual void BeginPlay() override;

private:
	void SetFirstNodeLocationInBlackBoard();

	bool HasReachedLastPoint = false;

	TArray<FVector> PathLocations;

	UPROPERTY(EditDefaultsOnly, Category = "PathFinding", meta = (AllowPrivateAccess = "true"))
		UBehaviorTree* BTEnemy;

	int32 CurrentLocationIndex = 0;

	UPurifiableComponent* GetPurifiableComponent() const;
	UTurnableComponent* GetTurnableComponent() const;
};
