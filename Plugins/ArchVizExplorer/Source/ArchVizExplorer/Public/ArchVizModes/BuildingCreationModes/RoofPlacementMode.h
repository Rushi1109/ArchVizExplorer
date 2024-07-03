// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingCreationSubModes.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "RoofPlacementMode.generated.h"

class ARoofActor;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API URoofPlacementMode : public UBuildingCreationSubMode {
	GENERATED_BODY()

public:
	URoofPlacementMode();

	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;

	void SetCurrentRoofActor(ARoofActor* Actor);

	void SetRoofActor(ARoofActor* InRoofActor);
	void BindWidgetDelegates();
protected:
	virtual void HandleFreeState() override;
	virtual void HandleOldEntityState() override;
	virtual void HandleNewEntityState() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Roof")
	TSubclassOf<ARoofActor> RoofActorRef;

private:
	void HandleLeftClickAction();
	void HandleRKeyPressAction();
	void HandleMKeyPressAction();
	void HandleDeleteKeyPressAction();

	UFUNCTION()
	void HandleNewButtonClick();

	UFUNCTION()
	void HandleDeleteButtonClick();

	UFUNCTION()
	void HandleClosePanelButtonClick();

	UFUNCTION()
	void HandleDimensionSliderValueChange(float InValue);

	ARoofActor* RoofActor;

	bool bNewFloorStart;
};
