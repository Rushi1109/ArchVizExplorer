// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BuildingCreation/BuildingCreationActor.h"
#include "RoofActor.generated.h"

class UProceduralMeshComponent;

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API ARoofActor : public ABuildingCreationActor {
	GENERATED_BODY()

public:
	friend class URoofPlacementMode;
	friend class USaveAndLoadMode;

	// Sets default values for this actor's properties
	ARoofActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStartLocation(const FVector& NewStartLocation);
	const FVector& GetStartLocation();

	void SetEndLocation(const FVector& NewEndLocation); 
	const FVector& GetEndLocation();

	void SetDimensions(const FVector& InDImensions);
	const FVector& GetDimensions() const;

	void SetOffset(const FVector& InOffset);
	const FVector& GetOffset() const;

	void UpdateRoofDimensionSlider();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Floor")
	UProceduralMeshComponent* ProceduralMeshComponent;

	FVector StartLocation;
	FVector EndLocation;
	FVector Dimensions;
	FVector Offset;

	void GenerateRoof();
	void DestroyRoof();
	void HandlePreviewingState();
	void HandleMovingState();
	void HandleGeneratingState();

	void AdjustDirection(FVector& RoofOffset);

	void UpdateRoofProperties();
	void HandleMaterialChange(FMaterialAsset MaterialAsset);
	virtual void ApplyMaterial() override;
};
