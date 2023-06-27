// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PathNodeNetworkSceneComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURORA_API UPathNodeNetworkSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathNodeNetworkSceneComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	const FVector GetLocationAtIndex(int32 Index) const;

	void InitNodeLocations();

	int32 GetNumberOfLocationsPerEnemy() const {return NumberOfLocationsPerEnemy;}
	int32 GetLocationsPerSegment() const {return LocationsPerSegment;}
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies", meta = (AllowPrivateAccess = "true"))
		int32 NumberOfLocationsPerEnemy = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Path", meta = (AllowPrivateAccess = "true"))
		int32 LocationsPerSegment = 3;

	UPROPERTY(EditAnywhere, Category = "Nodes", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (MakeEditWidget = true))
		TArray<FVector> NodeLocations;

	UPROPERTY(EditInstanceOnly, Category = "Nodes", meta = (AllowPrivateAccess = "true"))
		bool ResetNodes = true;

	void SortNodeLocations();
};
