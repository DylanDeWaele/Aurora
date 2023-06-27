// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IsLastCheckPointReached.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Aurora/Controllers/BaseEnemyController.h"

UBTTask_IsLastCheckPointReached::UBTTask_IsLastCheckPointReached()
{
	NodeName = TEXT("IsLastCheckPointTargeted");
}

EBTNodeResult::Type UBTTask_IsLastCheckPointReached::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseEnemyController* controller = Cast<ABaseEnemyController>(OwnerComp.GetAIOwner());

	if (controller)
	{
		controller->IncrementLocationIndex();

		if (!controller->IsNextLocationValid())
		{
			FName key{"IsLastCheckPointTargeted"};
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(key, true);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Succeeded;
}
