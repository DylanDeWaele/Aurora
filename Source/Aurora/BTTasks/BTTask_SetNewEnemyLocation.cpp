// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetNewEnemyLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Aurora/Controllers/BaseEnemyController.h"
#include "NavigationSystem.h"

UBTTask_SetNewEnemyLocation::UBTTask_SetNewEnemyLocation()
{
	NodeName = TEXT("SetNewEnemyLocation");
}

EBTNodeResult::Type UBTTask_SetNewEnemyLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseEnemyController* controller = Cast<ABaseEnemyController>(OwnerComp.GetAIOwner());

	if (controller)
	{
		UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		FNavLocation newLocation;
		bool foundPoint = navSystem->MainNavData->GetRandomReachablePointInRadius(controller->GetCurrentMovementLocation(), Radius, newLocation);

		if (foundPoint)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), newLocation.Location);
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}