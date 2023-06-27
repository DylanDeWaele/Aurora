// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_HasReachedLastCheckPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Aurora/Controllers/BaseEnemyController.h"
UBTTask_HasReachedLastCheckPoint::UBTTask_HasReachedLastCheckPoint()
{

}

EBTNodeResult::Type UBTTask_HasReachedLastCheckPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);

	return EBTNodeResult::Succeeded;
}
