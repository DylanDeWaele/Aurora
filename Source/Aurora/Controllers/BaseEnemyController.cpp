// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "Aurora/Components/TurnableComponent.h"
#include "Aurora/Components/PurifiableComponent.h"

ABaseEnemyController::ABaseEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseEnemyController::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	SetCanMoveInBlackboard();
}

void ABaseEnemyController::InitAndStartMoving_Implementation(const TArray<FVector>& locations, EEnemyType EnemyType)
{
	PathLocations = locations;

	//start behavior tree
	if (BTEnemy)
	{
		RunBehaviorTree(BTEnemy);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("behavior tree not found"));
	}

	//set enemyType
	auto enemy = Cast<ABaseEnemy>(GetPawn());

	if (enemy)
	{
		enemy->SetEnemyType(EnemyType);
	}

	//set player location
	SetPlayerLocationInBlackBoard();

	//set blackboard location
	SetFirstNodeLocationInBlackBoard();

	//increment so next time the next location is chosen
	IncrementLocationIndex();
}

const FVector& ABaseEnemyController::GetCurrentMovementLocation() const
{
	return PathLocations[CurrentLocationIndex];
}
void ABaseEnemyController::IncrementLocationIndex()
{
	++CurrentLocationIndex;
}

void ABaseEnemyController::SetFirstNodeLocationInBlackBoard()
{
	UBlackboardComponent* blackBoard = nullptr;

	if (BrainComponent)
	{
		blackBoard = BrainComponent->GetBlackboardComponent();
	}

	FName key = TEXT("WanderNodeLocation");

	if (blackBoard)
	{
		blackBoard->SetValueAsVector(key, PathLocations[0]);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("blackboard not found"));
	}
}

UPurifiableComponent* ABaseEnemyController::GetPurifiableComponent() const
{
	UActorComponent* actorComponent = GetPawn()->GetComponentByClass(UPurifiableComponent::StaticClass());

	if (actorComponent)
		return Cast<UPurifiableComponent>(actorComponent);

	return nullptr;
}

UTurnableComponent* ABaseEnemyController::GetTurnableComponent() const
{
	UActorComponent* actorComponent = GetPawn()->GetComponentByClass(UTurnableComponent::StaticClass());

	if (actorComponent)
		return Cast<UTurnableComponent>(actorComponent);

	return nullptr;
}

void ABaseEnemyController::SetPlayerLocationInBlackBoard()
{
	UBlackboardComponent* blackBoard = nullptr;

	if (BrainComponent)
	{
		blackBoard = BrainComponent->GetBlackboardComponent();
	}

	FName key = TEXT("PlayerLocation");

	if (blackBoard)
	{
		blackBoard->SetValueAsVector(key, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("blackboard not found"));
	}
}

void ABaseEnemyController::SetCanMoveInBlackboard()
{
	//Get Components
	UTurnableComponent* turnable = GetTurnableComponent();
	UPurifiableComponent* purifiable = GetPurifiableComponent();
	UBlackboardComponent* blackBoard = nullptr;

	if (BrainComponent)
	{
		blackBoard = BrainComponent->GetBlackboardComponent();
	}

	FName key = TEXT("CanMove");

	if (blackBoard)
	{
		if (purifiable && turnable)
			blackBoard->SetValueAsBool(key, !(purifiable->IsBeingPurified() || turnable->IsBeingTurned()));
		else
			blackBoard->SetValueAsBool(key, true);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("blackboard not found"));
	}
}

bool ABaseEnemyController::IsNextLocationValid() const
{
	if (CurrentLocationIndex >= PathLocations.Num())
	{
		return false;
	}
	else
	{
		return true;
	}
}
