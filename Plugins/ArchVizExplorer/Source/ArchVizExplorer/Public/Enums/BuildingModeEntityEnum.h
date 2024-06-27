// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingModeEntityEnum.generated.h"

UENUM()
enum class EBuildingModeEntity : uint8 {
	WallPlacement,
	DoorPlacement,
	FloorPlacement,
	RoofPlacement,
};
