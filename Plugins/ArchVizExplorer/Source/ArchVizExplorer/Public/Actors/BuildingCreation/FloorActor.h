// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingCreationActor.h"
#include "FloorActor.generated.h"

class UProceduralMeshComponent;

UCLASS()
class ARCHVIZEXPLORER_API AFloorActor : public ABuildingCreationActor {
	GENERATED_BODY()

public:
	friend class UFloorPlacementMode;

	// Sets default values for this actor's properties
	AFloorActor();

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

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Floor")
	UProceduralMeshComponent* ProceduralMeshComponent;

	FVector StartLocation;
	FVector EndLocation;

	void GenerateFloor(const FVector& Dimensions);
	void DestroyFloor();
	void HandlePreviewingState();
	void HandleMovingState();
	void HandleGeneratingState();
};
