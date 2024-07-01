// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizModeEnum.generated.h"

UENUM()
enum class EArchVizMode : uint8 {
	ViewMode,
	RoadConstruction,
	BuildingCreation,
	InteriorDesign,
	SaveAndLoad,
};
