// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetNewEnemyLocation.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API UBTTask_SetNewEnemyLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_SetNewEnemyLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "NewPoint", meta = (AllowPrivateAccess = "true"))
		float Radius = 50.f;
};
