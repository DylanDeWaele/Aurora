// Fill out your copyright notice in the Description page of Project Settings.


#include "StaffCrystals.h"
#include "Aurora/Characters/BaseVRCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStaffCrystals::AStaffCrystals()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = root;

	BigCrystal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BigCrystal"));
	BigCrystal->SetupAttachment(root);

	SmallCrystals = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SmallCrystals"));
	SmallCrystals->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AStaffCrystals::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ABaseVRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AStaffCrystals::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMaterial();
}

void AStaffCrystals::SetAbility(EAbility Type)
{
	CurrentAbility = Type;

	//Set materials on meshes
	switch (Type)
	{
	case EAbility::None:
		BigCrystal->SetMaterial(0, NeutralCrystalMaterial);
		SmallCrystals->SetMaterial(0, NeutralCrystalMaterial);

		BigCrystalDynamicMaterial = nullptr;
		SmallCrystalDynamicMaterial = nullptr;
		break;
	case EAbility::Kill:
		BigCrystal->SetMaterial(0, KillCrystalMaterial);
		SmallCrystals->SetMaterial(0, KillCrystalMaterial);

		//Create dynamic material instance so we can tweak the values later
		BigCrystalDynamicMaterial = BigCrystal->CreateDynamicMaterialInstance(0, KillCrystalMaterial);
		SmallCrystalDynamicMaterial = SmallCrystals->CreateDynamicMaterialInstance(0, KillCrystalMaterial);
		break;
	case EAbility::Purify:
		BigCrystal->SetMaterial(0, PurifyCrystalMaterial);
		SmallCrystals->SetMaterial(0, PurifyCrystalMaterial);

		//Create dynamic material instance so we can tweak the values later
		BigCrystalDynamicMaterial = BigCrystal->CreateDynamicMaterialInstance(0, PurifyCrystalMaterial);
		SmallCrystalDynamicMaterial = SmallCrystals->CreateDynamicMaterialInstance(0, PurifyCrystalMaterial);
		break;
	}
}

void AStaffCrystals::UpdateMaterial()
{
	if (SmallCrystalDynamicMaterial)
		SmallCrystalDynamicMaterial->SetScalarParameterValue("Mask Position", Player->GetCurrentAmmoPercentage() * 6.f);
	if (BigCrystalDynamicMaterial)
		BigCrystalDynamicMaterial->SetScalarParameterValue("Mask Position", Player->GetCurrentAmmoPercentage() * 6.f);
}
