#include "KillCrosshair.h"
#include "DrawDebugHelpers.h"

// Sets default values
AKillCrosshair::AKillCrosshair()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionChannel = ECC_Visibility;
}

// Called when the game starts or when spawned
void AKillCrosshair::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AKillCrosshair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}