// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateEnemyLocation.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API UBTService_UpdateEnemyLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateEnemyLocation();
private:
	const FName EnemyKey = TEXT("Enemy");
	const FName EnemyLocationKey = TEXT("EnemyLocation");
	const FName HasChasedPurifiedEnemy = TEXT("HasChasedPurifiedEnemy");

	bool CheckIfTurned(UBehaviorTreeComponent& OwnerComp) const;

	void FilterEnemies(bool IsTurned, TArray<AActor*>& NearbyEnemies);

	class UTurnableComponent* turnableComponent;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};
