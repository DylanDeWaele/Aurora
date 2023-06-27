// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Aurora/Actors/AudioManager.h"
#include "Aurora/Components/HealthComponent.h"
#include "Aurora/Components/KillableComponent.h"
#include "Aurora/Components/PurifiableComponent.h"
#include "Aurora/Components/TurnableComponent.h"
#include "Aurora/GameModes/AuroraGameModeBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Aurora/Enums/EnemyType.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "AIController.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	if (!IsValid(HealthComponent))
		GLog->Log("No healthComponent found");


	KillableComponent = CreateDefaultSubobject<UKillableComponent>(TEXT("KillableComponent"));
	if (!IsValid(KillableComponent))
		GLog->Log("No killableComponent found");

	//This is buggy -> create it in BP
	//PurifyImpactComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PurifyImpactParticles"));
	//PurifyImpactComponent->SetupAttachment(RootComponent);
	//PurifyImpactComponent->ComponentTags.Add("ImpactParticles");

	PurifiedEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PurifiedEffectComponent"));
	PurifiedEffectComponent->SetupAttachment(RootComponent);
	PurifiedEffectComponent->ComponentTags.Add("PurifiedParticles");

	EnemyDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyDetectionSphere"));
	EnemyDetectionSphere->SetupAttachment(GetRootComponent());

	EvaporateEvilEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EvaporateEvilEffectComponent"));
	EvaporateEvilEffectComponent->SetupAttachment(RootComponent);
	EvaporateEvilEffectComponent->ComponentTags.Add("EvaporateEvilParticles");

	EvaporateGoodEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EvaporateGoodEffectComponent"));
	EvaporateGoodEffectComponent->SetupAttachment(RootComponent);
	EvaporateGoodEffectComponent->ComponentTags.Add("EvaporateGoodParticles");
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerHealthRef();

	InitPurifiableComponent();
	InitTurnableComponent();

	SetupDetectionSphere();

	AudioManager = GetAudioManager();

	//Play a breathing sound
	if (IsValid(AudioManager))
		BreathingSound = AudioManager->PlaySound(EType::SFX, "Enemy_Breathing", 0, GetActorLocation());
}

void ABaseEnemy::SetTriggerBlackBoardValue(bool Enable)
{
	auto blackBoard = Cast<AAIController>(GetController())->BrainComponent->GetBlackboardComponent();
	if (blackBoard)
	{
		blackBoard->SetValueAsBool("IsTriggered", Enable);
	}
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckIsDead();

	if (BreathingSound && BreathingSound->IsPendingKill())
		BreathingSound = nullptr;

	//Let the breathing sound follow the player
	if (BreathingSound)
		BreathingSound->SetWorldLocation(GetActorLocation());
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

EEnemyType ABaseEnemy::GetEnemyType() const
{
	return EnemyType;
}

bool ABaseEnemy::GetIsTriggered() const
{
	return IsTriggered;
}

bool ABaseEnemy::GetHasEnemy() const
{
	if (Target == nullptr)
		return false;
	else
		return true;
}

UPurifiableComponent* ABaseEnemy::GetPurifiableComponent() const
{
	return PurifiableComponent;
}

UTurnableComponent* ABaseEnemy::GetTurnableComponent() const
{
	return TurnableComponent;
}

UHealthComponent* ABaseEnemy::GetHealthComponent() const
{
	return HealthComponent;
}

void ABaseEnemy::CheckIsTriggered()
{
}

void ABaseEnemy::CheckIsDead()
{
	if (!IsValid(HealthComponent))
		return;

	if (HealthComponent->IsDead())
	{
		if (BreathingSound)
			BreathingSound->Stop();

		TurnColliders(false);

		auto controller = Cast<AAIController>(GetController());
		UBlackboardComponent* blackBoard = nullptr;
		if (IsValid(controller))
		{
			auto brain = controller->BrainComponent;
			if (IsValid(brain))
			{
				blackBoard = brain->GetBlackboardComponent();
			}

			if (IsValid(blackBoard))
			{
				blackBoard->SetValueAsBool("IsDead", true);
			}
		}
	}
}

void ABaseEnemy::TurnColliders(bool Enable)
{
	UCapsuleComponent* collider = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));

	if (IsValid(collider))
	{
		if (Enable)
			collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		else
		{
			collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}


void ABaseEnemy::StopBreathing()
{
	if (BreathingSound)
		BreathingSound->Stop();
}

bool ABaseEnemy::IsBeingEvaporated() const
{
	return bIsBeingEvaporated;
}

void ABaseEnemy::SetupEvaporateParticles()
{
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	playerLocation.Z += 200.f;
	playerLocation.X = 0.f;
	playerLocation.Y = 0.f;

	GLog->Log(playerLocation.ToString());

	EvaporateEvilEffectComponent->SetVectorParameter("PlayerLocation",playerLocation);
	EvaporateGoodEffectComponent->SetVectorParameter("PlayerLocation", playerLocation);
}

void ABaseEnemy::SetEnemyInBlackboard(ABaseEnemy* Enemy)
{
	auto controller = Cast<AAIController>(GetController());
	UBlackboardComponent* blackboard = nullptr;

	if (IsValid(controller))
	{
		auto brainComponent = controller->BrainComponent;
		if (IsValid(brainComponent))
		{
			blackboard = brainComponent->GetBlackboardComponent();
		}
	}

	if (IsValid(blackboard))
	{
		if (IsValid(Enemy))
		{
			blackboard->SetValueAsObject("Enemy", Enemy);
			blackboard->SetValueAsVector("EnemyLocation", Enemy->GetActorLocation());
		}
	}
}

void ABaseEnemy::SetupDetectionSphere()
{
	EnemyDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABaseEnemy::AttackEnemyTarget(int32 HitAnimIndex, int32 Damage)
{
	auto target = Cast<ABaseEnemy>(Cast<AAIController>(GetController())->BrainComponent->GetBlackboardComponent()->GetValueAsObject("Enemy"));

	if (IsValid(target))
	{
		target->GetHit(HitAnimIndex, Damage);
	}
}

void ABaseEnemy::PlayEvaporateGoodEffect()
{
	FRotator hmdRotator;
	FVector hmdPosition;
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(hmdRotator, hmdPosition);

	auto playerLocation = hmdPosition - GetActorLocation();
	playerLocation.Normalize();

	playerLocation = playerLocation.RotateAngleAxis(95.f, FVector::UpVector);

	float distance = 220.f;
	playerLocation *= distance;
	playerLocation.Z += 100.f;

	EvaporateGoodEffectComponent->SetVectorParameter("PlayerLocation", playerLocation);

	EvaporateGoodEffectComponent->Activate();
	AudioManager->PlaySound(EType::SFX, "Player_Heal");

	bIsBeingEvaporated = true;
}

void ABaseEnemy::PlayEvaporateEvilEffect()
{
	FRotator hmdRotator;
	FVector hmdPosition;
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(hmdRotator, hmdPosition);

	auto playerLocation = hmdPosition - GetActorLocation();
	playerLocation.Normalize();

	playerLocation = playerLocation.RotateAngleAxis(90.f, FVector::UpVector);

	float distance = 240.f;
	playerLocation *= distance;
	playerLocation.Z += 100.f;

	EvaporateEvilEffectComponent->SetVectorParameter("PlayerLocation", playerLocation);

	EvaporateEvilEffectComponent->Activate();
	AudioManager->PlaySound(EType::SFX, "Player_Damage");

	bIsBeingEvaporated = true;
}

void ABaseEnemy::EvilEvaporateSelf()
{
	Cast<AAIController>(GetController())->SetFocus(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerHealthRef)
	{
		PlayerHealthRef->DecreaseHealth(EvaporateDamage);
	}

	if (BreathingSound)
		BreathingSound->Stop();

	if (DeathCloud)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathCloud, GetActorLocation());

	GetMesh()->SetHiddenInGame(true);
	TurnColliders(false);
	
	GetWorld()->GetTimerManager().SetTimer(KillSelf, this, &ABaseEnemy::DestroySelf, 2.f, false);
}

void ABaseEnemy::GoodEvaporateSelf()
{
	if (PlayerHealthRef)
	{
		PlayerHealthRef->IncreaseHealth(EvaporateDamage);
	}

	if (BreathingSound)
		BreathingSound->Stop();

	if (DeathCloud)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathCloud, GetActorLocation());

	GetMesh()->SetHiddenInGame(true);
	TurnColliders(false);

	GetWorld()->GetTimerManager().SetTimer(KillSelf, this, &ABaseEnemy::DestroySelf, 2.f, false);

}

void ABaseEnemy::DestroySelf()
{
	Destroy();
}

void ABaseEnemy::InitPlayerHealthRef()
{
	auto player = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	if (player)
	{
		auto healthComponent = Cast<UHealthComponent>(player->GetComponentByClass(UHealthComponent::StaticClass()));
		if (IsValid(healthComponent))
		{
			PlayerHealthRef = healthComponent;
		}
	}
}

void  ABaseEnemy::InitPurifiableComponent()
{
	UPurifiableComponent* purifiable = Cast<UPurifiableComponent>(GetComponentByClass(UPurifiableComponent::StaticClass()));

	if (IsValid(purifiable))
	{
		PurifiableComponent = purifiable;
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("No purifiable component found on enemy."));
}

void  ABaseEnemy::InitTurnableComponent()
{
	UTurnableComponent* turnable = Cast<UTurnableComponent>(GetComponentByClass(UTurnableComponent::StaticClass()));

	if (IsValid(turnable))
	{
		TurnableComponent = turnable;
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("No turnable component found on enemy."));
}

//Animation Event when this gets hit
void ABaseEnemy::GetHit(int32 HitAnimIndex, int32 Damage)
{
	if (HitAnimIndex >= GetHitAnims.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("hit animation missing"));
		return;
	}
	PlayAnimMontage(GetHitAnims[HitAnimIndex]);

	if (IsValid(HealthComponent))
		HealthComponent->DecreaseHealth(Damage);
}

void ABaseEnemy::SetEnemyPath(const TArray<FVector>& locations)
{
	//expensive copy
	Locations = locations;
}

void ABaseEnemy::SetEnemyType(EEnemyType Type)
{
	EnemyType = Type;
}

AAudioManager* ABaseEnemy::GetAudioManager() const
{
	//Get audio manager
	AAuroraGameModeBase* auroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (auroraGameMode)
		return auroraGameMode->GetAudioManager();

	UE_LOG(LogTemp, Warning, TEXT("Current gamemode is not the Aurora game mode."));
	return nullptr;
}