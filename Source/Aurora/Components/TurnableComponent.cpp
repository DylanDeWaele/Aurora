// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnableComponent.h"

#include "Aurora/Actors/TurnBomb.h"
#include "Aurora/Controllers/BaseEnemyController.h"
#include "Aurora/Characters/BaseEnemy.h"
#include "Aurora/Components/PurifiableComponent.h"
#include "Aurora/Components/HealthComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"


UTurnableComponent::UTurnableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTurnableComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeDynamicMaterials();

	TurnedParticles = GetTurnedParticles();
}

void UTurnableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPurifiableComponent* purifiableComponent = Owner->GetPurifiableComponent();
	UHealthComponent* healthComponent = Owner->GetHealthComponent();
	bool beingEvaporated = Cast<ABaseEnemy>(Owner)->IsBeingEvaporated();

	if (!beingEvaporated && !bIsTurned && purifiableComponent && healthComponent)
	{
		if (!purifiableComponent->IsPurified() && !healthComponent->IsDead())
		{
			CheckIfBeingTurned();
			HandleTurning(DeltaTime);
		}
	}
}

bool UTurnableComponent::IsBeingTurned() const
{
	return bIsBeingTurned;
}

bool UTurnableComponent::IsTurned() const
{
	return bIsTurned;
}

void UTurnableComponent::CheckIfBeingTurned()
{
	TArray<AActor*> overlappingActors{};

	//get capsule collider because you don't want to check collision with other colliders
	auto collision = Cast<UCapsuleComponent>(GetOwner()->GetComponentByClass(UCapsuleComponent::StaticClass()));
	if (IsValid(collision))
	{
		collision->GetOverlappingActors(overlappingActors);
	}

	for (AActor* actor : overlappingActors)
	{
		ATurnBomb* bomb = Cast<ATurnBomb>(actor);

		if (bomb)
		{
			bIsBeingTurned = true;
			return;
		}
	}

	bIsBeingTurned = false;
}

void UTurnableComponent::HandleTurning(float DeltaTime)
{
	if (bIsBeingTurned)
	{
		CurrentTurnPercentage += TurnSpeed * DeltaTime;
		UpdateMaterials();

		if (CurrentTurnPercentage >= 100)
			Turn();
	}
	else
	{
		//When the actor is not being purified we should decay the current purification percentage
		if (CurrentTurnPercentage > 0)
		{
			CurrentTurnPercentage -= DecaySpeed * DeltaTime;
			UpdateMaterials();
		}
	}
}

void UTurnableComponent::Turn()
{
	SetIsTurnedInBlackBoard();

	bIsBeingTurned = false;
	bIsTurned = true;

	TurnedParticles->Activate();

	AddToStats(EAbility::Turn, 1);
}

void UTurnableComponent::SetIsTurnedInBlackBoard()
{
	auto blackboard = Cast<AAIController>(Cast<ABaseEnemy>(GetOwner())->GetController())->GetBlackboardComponent();

	if (blackboard)
	{
		blackboard->SetValueAsBool("IsTurned", true);
	}
}

void UTurnableComponent::InitializeDynamicMaterials()
{
	//Get skeletal mesh
	UActorComponent* actorComponent = GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass());
	if (actorComponent)
	{
		for (int i = 0; i < AmountOfMaterials; i++)
		{
			USkeletalMeshComponent* skeletalMesh = Cast<USkeletalMeshComponent>(actorComponent);

			if (skeletalMesh)
			{
				//Get the material
				UMaterialInterface* material = skeletalMesh->GetMaterial(i);

				if (material)
					DynamicMaterials.Add(skeletalMesh->CreateDynamicMaterialInstance(i, material));
			}
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("No skeletal mesh component found on an actor that has a turned component attached to it."));
}

void UTurnableComponent::UpdateMaterials()
{
	if (DynamicMaterials.Num() > 0)
	{
		for (UMaterialInstanceDynamic* dynamicMaterial : DynamicMaterials)
		{
			dynamicMaterial->SetScalarParameterValue("GoodPercentage", CurrentTurnPercentage / 100.f);
		}
	}
}

UNiagaraComponent* UTurnableComponent::GetTurnedParticles() const
{
	TArray<UActorComponent*> particles = GetOwner()->GetComponentsByTag(UNiagaraComponent::StaticClass(), "TurnedParticles");

	for (UActorComponent* component : particles)
	{
		return Cast<UNiagaraComponent>(component);
	}

	return nullptr;
}