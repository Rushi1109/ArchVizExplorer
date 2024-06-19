// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/ArchVizUtility.h"

ArchVizUtility::ArchVizUtility() {
}

ArchVizUtility::~ArchVizUtility() {
}

FVector ArchVizUtility::SnapToGrid(const FVector& WorldLocation) {
	float GridSize = 100.0f;
	float SnappedX = FMath::RoundToFloat(WorldLocation.X / GridSize) * GridSize;
	float SnappedY = FMath::RoundToFloat(WorldLocation.Y / GridSize) * GridSize;
	float SnappedZ = FMath::RoundToFloat(WorldLocation.Z / GridSize) * GridSize;

	return FVector(SnappedX, SnappedY, SnappedZ);
}