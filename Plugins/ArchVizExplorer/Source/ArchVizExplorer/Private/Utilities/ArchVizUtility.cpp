// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/ArchVizUtility.h"

ArchVizUtility::ArchVizUtility() {
}

ArchVizUtility::~ArchVizUtility() {
}

FVector ArchVizUtility::SnapToGrid(const FVector& WorldLocation, const FVector& GridSize) {
	float SnappedX = FMath::RoundToFloat(WorldLocation.X / GridSize.X) * GridSize.X;
	float SnappedY = FMath::RoundToFloat(WorldLocation.Y / GridSize.Y) * GridSize.Y;
	float SnappedZ = FMath::RoundToFloat(WorldLocation.Z / GridSize.Z) * GridSize.Z;

	return FVector(SnappedX, SnappedY, SnappedZ);
}

FVector ArchVizUtility::SnapToGridFloored(const FVector& WorldLocation, const FVector& GridSize) {
	float SnappedX = FMath::FloorToFloat(WorldLocation.X / GridSize.X) * GridSize.X;
	float SnappedY = FMath::FloorToFloat(WorldLocation.Y / GridSize.Y) * GridSize.Y;
	float SnappedZ = FMath::RoundToFloat(WorldLocation.Z / GridSize.Z) * GridSize.Z;

	return FVector(SnappedX, SnappedY, SnappedZ);
}