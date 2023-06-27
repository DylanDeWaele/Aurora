// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEnemySpawner.generated.h"

class ABaseEnemy;
class APathNodeNetwork;
class UPathNodeNetworkSceneComponent;
class UBoxComponent;

enum class EEnemyType : uint8;

UCLASS()
class AURORA_API ABaseEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemySpawner();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void SpawnEnemy(TSubclassOf<ABaseEnemy> EnemyToSpawn, EEnemyType TypeOfEnemy);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Path", meta = (AllowPrivateAccess = "true"))
		UPathNodeNetworkSceneComponent* PathNodeNetworkComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxCollider;

};
