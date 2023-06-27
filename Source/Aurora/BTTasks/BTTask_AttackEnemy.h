// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURORA_API UBTTask_AttackEnemy : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_AttackEnemy();
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	const FName EnemyKey = TEXT("Enemy");

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true "))
		int32 DamageAmount = 5;

	//The order needs to be numbered order
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> AnimMontages;

protected:

};
