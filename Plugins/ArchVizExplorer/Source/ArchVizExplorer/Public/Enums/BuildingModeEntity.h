// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingModeEntity.generated.h"

UENUM()
enum class EBuildingModeEntity : uint8 {
	None,
	WallPlacement,
	DoorPlacement,
	FloorPlacement,
	RoofPlacement,
};
