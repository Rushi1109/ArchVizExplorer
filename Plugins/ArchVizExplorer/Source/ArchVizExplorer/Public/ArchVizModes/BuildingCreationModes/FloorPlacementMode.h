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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wall")
	TSubclassOf<AFloorActor> FloorActorRef;

private:
	UFUNCTION()
	void HandleLeftClickAction();

	UFUNCTION()
	void HandleRKeyPressAction();

	UFUNCTION()
	void HandleMKeyPressAction();

	UPROPERTY(VisibleDefaultsOnly, Category = "Wall")
	AFloorActor* FloorActor;

	bool bNewFloorStart;
};
