// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizMode.h"
#include "UObject/NoExportTypes.h"
#include "Enums/BuildingModeEntityEnum.h"
#include "BuildingCreationModes/BuildingCreationModes.h"
#include "BuildingCreationModes/WallPlacementMode.h"
#include "InputMappingContext.h"
#include "BuildingCreationMode.generated.h"

class AWallActor;
class APlayerController;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API UBuildingCreationMode : public UObject, public IArchVizMode {
	GENERATED_BODY()

public:
	UBuildingCreationMode();

	void SetupSubModes();

	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;

	void InitParam(APlayerController* Controller);

	void SetSubMode(IBuildingCreationMode* NewSubMode);

	void SetBuildingModeEntity(EBuildingModeEntity Entity);

	void PreviewSegment();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BuildingCreationMode | Wall")
	TSubclassOf<UWallPlacementMode> WallPlacementModeRef;

private:
	IBuildingCreationMode* CurrentBuildingCreationSubMode;

	UPROPERTY()
	EBuildingModeEntity BuildingModeEntity;

	UPROPERTY(VisibleDefaultsOnly, Category = "BuildingCreationMode | Wall")
	UWallPlacementMode* WallPlacementMode;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;
};
