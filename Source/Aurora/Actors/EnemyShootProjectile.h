// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyShootProjectile.generated.h"

class USphereComponent;
class UMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class AURORA_API AEnemyShootProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyShootProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	//Components
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USphereComponent* SphereCollider;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"), BlueprintReadOnly)
		float ProjectileSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"), BlueprintReadOnly)
		float DamageAmount = 10.f;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		void Destruction();

};
