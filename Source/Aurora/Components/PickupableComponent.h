// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURORA_API UPickupableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickupableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void PickUp(USceneComponent* ComponentToAttachTo);
	void Drop();

	bool IsGrabbed() const;

private:
	UStaticMeshComponent* MeshComponent;

	bool bIsGrabbed = false;
};