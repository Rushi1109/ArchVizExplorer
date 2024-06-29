// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizMode.h"
#include "UObject/NoExportTypes.h"
#include "Enums/BuildingModeEntityEnum.h"
#include "BuildingCreationModes/BuildingCreationSubModes.h"
#include "BuildingCreationModes/WallPlacementMode.h"
#include "BuildingCreationModes/DoorPlacementMode.h"
#include "BuildingCreationModes/FloorPlacementMode.h"
#include "BuildingCreationModes/RoofPlacementMode.h"
#include "InputMappingContext.h"
#include "Widgets/BuildingCreationWidget.h"
#include "BuildingCreationMode.generated.h"

class AWallActor;
class APlayerController;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API UBuildingCreationMode : public UArchVizMode {
	GENERATED_BODY()

public:
	UBuildingCreationMode();

	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;

	void SetSubMode(UBuildingCreationSubMode* NewSubMode);

	void HandleBuildingSubModeChange(EBuildingModeEntity NewBuildingModeEntity);
	void HandleBuildingActorSelected(EBuildingModeEntity NewBuildingModeEntity, AActor* Actor);
	void UpdateBuildingModeEntity();
	void UpdateSelectedActor(AActor* Actor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BuildingCreationMode | Wall")
	TSubclassOf<UWallPlacementMode> WallPlacementModeRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BuildingCreationMode | Door")
	TSubclassOf<UDoorPlacementMode> DoorPlacementModeRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BuildingCreationMode | Floor")
	TSubclassOf<UFloorPlacementMode> FloorPlacementModeRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BuildingCreationMode | Roof")
	TSubclassOf<URoofPlacementMode> RoofPlacementModeRef;


private:
	UBuildingCreationSubMode* CurrentBuildingCreationSubMode;

	UPROPERTY()
	EBuildingModeEntity BuildingModeEntity;

	UPROPERTY()
	UWallPlacementMode* WallPlacementMode;

	UPROPERTY()
	UDoorPlacementMode* DoorPlacementMode;

	UPROPERTY()
	UFloorPlacementMode* FloorPlacementMode;

	UPROPERTY()
	URoofPlacementMode* RoofPlacementMode;
};
