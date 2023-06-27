// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DestroySelf.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API UDestroySelf : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UDestroySelf();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
};
