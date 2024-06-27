// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingCreationSubModes.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "Actors/BuildingCreation/DoorActor.h"
#include "DoorPlacementMode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API UDoorPlacementMode : public UBuildingCreationSubMode {
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;

	void SetCurrentDoorActor(ADoorActor* Actor);
protected:
	virtual void HandleFreeState() override;
	virtual void HandleOldEntityState() override;
	virtual void HandleNewEntityState() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADoorActor> DoorActorRef;

private:
	void HandleLeftClickAction();
	void HandleRKeyPressAction();
	void HandleMKeyPressAction();
	void HandleOKeyPressAction();
	void HandleDeleteKeyPressAction();

	void BindWidgetDelegates();

	UFUNCTION()
	void HandleNewButtonClick();

	UFUNCTION()
	void HandleDeleteButtonClick();

	UFUNCTION()
	void HandleClosePanelButtonClick();

	UPROPERTY()
	ADoorActor* DoorActor;
};
