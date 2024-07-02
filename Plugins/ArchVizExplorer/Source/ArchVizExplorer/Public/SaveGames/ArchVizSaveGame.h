// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteriorActor.h"
#include "Actors/RoadActor.h"
#include "GameFramework/SaveGame.h"
#include "ArchVizSaveGame.generated.h"


USTRUCT()
struct FRoadData {

	GENERATED_BODY()

	FRoadData();

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<FVector> SplinePoints;

	UPROPERTY()
	ERoadPointType PointType;

	UPROPERTY()
	float Width;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	int32 ParentActorId;
};

USTRUCT()
struct FWallData {

	GENERATED_BODY()

	FWallData();

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	double Length;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	int32 ParentActorId;
};

USTRUCT()
struct FFloorData {
	GENERATED_BODY()

	FFloorData();

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Dimensions;

	UPROPERTY()
	FVector Offset;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector EndLocation;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	int32 ParentActorId;
};

USTRUCT()
struct FRoofData {
	GENERATED_BODY()

	FRoofData();

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Dimensions;

	UPROPERTY()
	FVector Offset;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector EndLocation;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	int32 ParentActorId;
};


USTRUCT()
struct FDoorData {
	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	bool bIsOpen;

	UPROPERTY()
	int32 ParentActorId;

	UPROPERTY()
	int32 ParentComponentIndex;
};


USTRUCT()
struct FInteriorData {
	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	UStaticMesh* StaticMesh;

	UPROPERTY()
	EInteriorAssetType Category;

	UPROPERTY()
	int32 ParentActorId;
};

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UArchVizSaveGame : public USaveGame {
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FRoadData> RoadActorArray;

	UPROPERTY()
	TArray<FWallData> WallActorArray;

	UPROPERTY()
	TArray<FFloorData> FloorActorArray;

	UPROPERTY()
	TArray<FRoofData> RoofActorArray;

	UPROPERTY()
	TArray<FDoorData> DoorActorArray;

	UPROPERTY()
	TArray<FInteriorData> InteriorActorArray;
};
