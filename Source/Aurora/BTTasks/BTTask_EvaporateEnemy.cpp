// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_EvaporateEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Aurora/Controllers/BaseEnemyController.h"
#include "Aurora/Components/HealthComponent.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"

UBTTask_EvaporateEnemy::UBTTask_EvaporateEnemy()
{
	NodeName = TEXT("EvaporateEnemy");
}

EBTNodeResult::Type UBTTask_EvaporateEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto owner = OwnerComp.GetAIOwner()->GetCharacter();

	if (owner)
	{
		if (!IsPurified)
		{
			owner->PlayAnimMontage(AnimationMontage, 1.f, FName{"Evil"});
			Cast<ABaseEnemy>(owner)->PlayEvaporateEvilEffect();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsDead", true);
		}
		else
		{
			owner->PlayAnimMontage(AnimationMontage, 1.f, FName{"Good"});
			Cast<ABaseEnemy>(owner)->PlayEvaporateGoodEffect();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsDead", true);
		}
	}

	return EBTNodeResult::Succeeded;
}
