// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingCreationSubModes.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "FloorPlacementMode.generated.h"

class AFloorActor;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API UFloorPlacementMode : public UBuildingCreationSubMode {
	GENERATED_BODY()

public:
	UFloorPlacementMode();

	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;

	void SetCurrentFloorActor(AFloorActor* Actor);

	void SetFloorActor(AFloorActor* InFloorActor);
	void BindWidgetDelegates();
protected:
	virtual void HandleFreeState() override;
	virtual void HandleOldEntityState() override;
	virtual void HandleNewEntityState() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Floor")
	TSubclassOf<AFloorActor> FloorActorRef;

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
	
	AFloorActor* FloorActor;

	bool bNewFloorStart;
};
