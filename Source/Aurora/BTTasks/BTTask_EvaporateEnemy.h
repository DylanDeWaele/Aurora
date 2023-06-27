// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EvaporateEnemy.generated.h"

UCLASS()
class AURORA_API UBTTask_EvaporateEnemy : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_EvaporateEnemy();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AnimationMontage;

	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool IsPurified;
};
