// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateEnemyLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "Aurora/Components/TurnableComponent.h"
#include "Aurora/Components/PurifiableComponent.h"
#include "Aurora/Components/HealthComponent.h"
#include "AIController.h"

UBTService_UpdateEnemyLocation::UBTService_UpdateEnemyLocation()
{
	NodeName = TEXT("UpdateEnemyLocation");

}

bool UBTService_UpdateEnemyLocation::CheckIfTurned(UBehaviorTreeComponent& OwnerComp) const
{
	auto turnComponent = Cast<UTurnableComponent>(OwnerComp.GetAIOwner()->GetPawn()->GetComponentByClass(UTurnableComponent::StaticClass()));

	if (IsValid(turnComponent))
	{
		if(turnComponent->IsTurned())
			return true;
		else
			return false;
	}
	return false;
}
//add all the possible targets to a array
void UBTService_UpdateEnemyLocation::FilterEnemies(bool IsTurned, TArray<AActor*>& NearbyEnemies)
{
	TArray<AActor*> indices;
	
	if (!IsTurned)
	{
		for (auto nearbyEnemy : NearbyEnemies)
		{
			bool isDead = Cast<UHealthComponent>(nearbyEnemy->GetComponentByClass(UHealthComponent::StaticClass()))->IsDead();

			if(isDead)
				continue;

			if (Cast<UTurnableComponent>(nearbyEnemy->GetComponentByClass(UTurnableComponent::StaticClass()))->IsTurned())
			{
				indices.Add(nearbyEnemy);
			}
		}
	}
	else
	{
		for (int32 i{}; i < NearbyEnemies.Num(); ++i)
		{
			if (!Cast<UTurnableComponent>(NearbyEnemies[i]->GetComponentByClass(UTurnableComponent::StaticClass()))->IsTurned()
				&& !Cast<UPurifiableComponent>(NearbyEnemies[i]->GetComponentByClass(UPurifiableComponent::StaticClass()))->IsPurified()
				&& !Cast<UHealthComponent>(NearbyEnemies[i]->GetComponentByClass(UHealthComponent::StaticClass()))->IsDead()
				&& !Cast<ABaseEnemy>(NearbyEnemies[i])->IsBeingEvaporated()
				&& !Cast<UTurnableComponent>(NearbyEnemies[i]->GetComponentByClass(UTurnableComponent::StaticClass()))->IsBeingTurned())
			{
				indices.Add(NearbyEnemies[i]);
			}
		}
	}

	NearbyEnemies = indices;
}

//this function can be a bit confusing
void UBTService_UpdateEnemyLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TArray<AActor*> closeEnemies;

	OwnerComp.GetAIOwner()->GetPawn()->GetOverlappingActors(closeEnemies, ABaseEnemy::StaticClass());

	FilterEnemies(CheckIfTurned(OwnerComp), closeEnemies);

	auto enemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(EnemyKey));

	if (closeEnemies.Num() == 0)
	{
		//if you have an enemy -> set enemy to nullptr because this means that enemy is outside of the sphere
		if (enemy)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(EnemyKey, nullptr);
		}
		return;
	}

	if (enemy)
	{
		//if you have an enemy
		for (auto i : closeEnemies)
		{
			if (i == enemy)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(EnemyLocationKey, enemy->GetActorLocation());
				return;
			}
		}

		//if enemy is gone set new enemy
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(EnemyKey, closeEnemies[0]);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(EnemyLocationKey, closeEnemies[0]->GetActorLocation());

		auto purify = Cast<UPurifiableComponent>(closeEnemies[0]->GetComponentByClass(UPurifiableComponent::StaticClass()));

		if (IsValid(purify))
		{
			if (purify->IsPurified())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(HasChasedPurifiedEnemy, true);
			}
		}
		return;
	}

	if (!enemy)
	{
		auto purify = Cast<UPurifiableComponent>(closeEnemies[0]->GetComponentByClass(UPurifiableComponent::StaticClass()));

		if (IsValid(purify))
		{
			if (purify->IsPurified())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(HasChasedPurifiedEnemy, true);
			}
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(EnemyKey, closeEnemies[0]);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(EnemyLocationKey, closeEnemies[0]->GetActorLocation());
	}

}
