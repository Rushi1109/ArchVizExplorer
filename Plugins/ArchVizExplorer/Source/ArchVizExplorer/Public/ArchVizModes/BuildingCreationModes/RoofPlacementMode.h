// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingCreationSubModes.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "RoofPlacementMode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API URoofPlacementMode : public UBuildingCreationSubMode {
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;

private:
	UFUNCTION()
	void HandleLeftMouseClick();
};
