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

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADoorActor> DoorActorRef;

private:
	void HandleLeftClickAction();
	void HandleRKeyPressAction();
	void HandleMKeyPressAction();
	void HandleOKeyPressAction();

	UPROPERTY()
	ADoorActor* DoorActor;
};
