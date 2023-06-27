// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupableComponent.h"

// Sets default values for this component's properties
UPickupableComponent::UPickupableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UPickupableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	TArray<UActorComponent*> grabMeshes = GetOwner()->GetComponentsByTag(UStaticMeshComponent::StaticClass(), "GrabMesh");
	if (grabMeshes.Num() > 0)
		MeshComponent = Cast<UStaticMeshComponent>(grabMeshes[0]);
	else
		UE_LOG(LogTemp, Warning, TEXT("No mesh with tag GrabMesh found on an object that has a pickupable component."));
}

void UPickupableComponent::PickUp(USceneComponent* ComponentToAttachTo)
{
	//First detach from previous actor
	GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	MeshComponent->SetSimulatePhysics(false);

	bIsGrabbed = true;

	GetOwner()->GetRootComponent()->AttachToComponent(ComponentToAttachTo, FAttachmentTransformRules::KeepWorldTransform);
}

void UPickupableComponent::Drop()
{
	MeshComponent->SetSimulatePhysics(true);

	bIsGrabbed = false;

	GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

bool UPickupableComponent::IsGrabbed() const
{
	return bIsGrabbed;
}
