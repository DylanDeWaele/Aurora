// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroySelf.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UDestroySelf::UDestroySelf()
{
	NodeName = TEXT("DestroySelf");
}

EBTNodeResult::Type UDestroySelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->Destroy();
	return EBTNodeResult::Succeeded;
}
