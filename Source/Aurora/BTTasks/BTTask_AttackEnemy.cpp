// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "Aurora/Components/HealthComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIController.h"

UBTTask_AttackEnemy::UBTTask_AttackEnemy()
{
	NodeName = TEXT("AttackEnemy");
}

EBTNodeResult::Type UBTTask_AttackEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto owner = OwnerComp.GetAIOwner()->GetCharacter();
	if (owner)
	{
		auto animInstance = owner->GetMesh()->GetAnimInstance();

		if (animInstance)
		{
			if (animInstance->Montage_IsActive(nullptr))
			{
				return EBTNodeResult::Failed;
			}

			//get random animation montage
			int32 randomIndex = FMath::RandRange(0, AnimMontages.Num() - 1);

			owner->PlayAnimMontage(AnimMontages[randomIndex]);
		}
	}
	return EBTNodeResult::Succeeded;
}
