// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IsLastCheckPointReached.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API UBTTask_IsLastCheckPointReached : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_IsLastCheckPointReached();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
