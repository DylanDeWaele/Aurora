// Fill out your copyright notice in the Description page of Project Settings.


#include "StaffHolder.h"
#include "Kismet/GameplayStatics.h"
#include "Aurora/Actors/Staff.h"
#include "Aurora/Characters/BaseVRCharacter.h"
#include "DrawDebugHelpers.h"

// Sets default values
AStaffHolder::AStaffHolder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	StaffSocket = CreateDefaultSubobject<USceneComponent>(TEXT("StaffSocket"));
	StaffSocket->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AStaffHolder::BeginPlay()
{
	Super::BeginPlay();

	Staff = UGameplayStatics::GetActorOfClass(GetWorld(), AStaff::StaticClass());
	Player = Cast<ABaseVRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	TeleportStaffBack();
}

// Called every frame
void AStaffHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Staff && Player)
	{
		if (StaffOutOfRange() && Staff->GetVelocity().IsNearlyZero() && (!Player->LeftHandHoldingStaff() && !Player->RightHandHoldingStaff()))
			TeleportStaffBack();
	}
}

bool AStaffHolder::StaffOutOfRange() const
{
	return (Player->GetActorLocation() - Staff->GetActorLocation()).Size() > TeleportDistance;
}

void AStaffHolder::TeleportStaffBack()
{
	TArray<UActorComponent*> meshes = Staff->GetComponentsByTag(UStaticMeshComponent::StaticClass(), "GrabMesh");
	if (meshes.Num() > 0)
		Cast<UStaticMeshComponent>(meshes[0])->SetSimulatePhysics(false);

	//Attach the staff to the socket
	Staff->AttachToComponent(StaffSocket, FAttachmentTransformRules::KeepWorldTransform);

	if (!Staff->SetActorLocationAndRotation(StaffSocket->GetComponentLocation(), StaffSocket->GetComponentRotation()))
		UE_LOG(LogTemp, Warning, TEXT("Staff failed to teleport!"));
}

