// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/ArchVizModeEnum.h"
#include "Enums/BuildingModeEntityEnum.h"
#include "ArchVizModes/ArchVizMode.h"
#include "ArchVizModes/RoadConstructionMode.h"
#include "ArchVizModes/BuildingCreationMode.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/ModesMenuWidget.h"
#include "Widgets/RoadConstructionWidget.h"
#include "Widgets/BuildingCreationWidget.h"
#include "Widgets/InteriorDesignWidget.h"
#include "ArchVizController.generated.h"

class ARoadActor;
class AWallActor;
class UInputMappingContext;

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API AArchVizController : public APlayerController {
	GENERATED_BODY()

public:
	AArchVizController();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	IArchVizMode* CurrentMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArchVizMode | ModesMenu")
	TSubclassOf<UModesMenuWidget> ModesMenuWidgetRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RoadConstruction | Widget")
	TSubclassOf<URoadConstructionWidget> RoadConstructionWidgetRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RoadConstruction | Mode")
	TSubclassOf<URoadConstructionMode> RoadConstructionModeRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BuildingCreation | Widget")
	TSubclassOf<UBuildingCreationWidget> BuildingCreationWidgetRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BuildingCreation | Mode")
	TSubclassOf<UBuildingCreationMode> BuildingCreationModeRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InteriorDesign | Widget")
	TSubclassOf<UInteriorDesignWidget> InteriorDesignWidgetRef;

private:
	UFUNCTION()
	void HandleModeChange(EArchVizMode ArchVizMode);

	void SetArchVizMode(IArchVizMode* NewMode);

	UFUNCTION()
	void HandleBuildingModeEntityChange(EBuildingModeEntity NewBuildingModeEntity);

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizMode")
	EArchVizMode ArchVizMode;

	UPROPERTY(VisibleDefaultsOnly, Category = "BuildingCreation | Entity", meta = (EditCondition = "ArchVizMode == EArchVizMode::BuildingCreation"))
	EBuildingModeEntity BuildingModeEntity;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizMode | ModesMenu")
	UModesMenuWidget* ModesMenuWidget;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadConstruction | Widget")
	URoadConstructionWidget* RoadConstructionWidget;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadConstruction | Mode")
	URoadConstructionMode* RoadConstructionMode;

	UPROPERTY(VisibleDefaultsOnly, Category = "BuildingCreation | Widget")
	UBuildingCreationWidget* BuildingCreationWidget;

	UPROPERTY(VisibleDefaultsOnly, Category = "BuildingCreation | Mode")
	UBuildingCreationMode* BuildingCreationMode;

	UPROPERTY(VisibleDefaultsOnly, Category = "InteriorDesign | Widget")
	UInteriorDesignWidget* InteriorDesignWidget;

	void CleanBeforeChange();
	void UpdateWidgets();

	FInputModeGameAndUI InputMode;
};
