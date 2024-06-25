// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingCreationSubModes.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "WallPlacementMode.generated.h"

class AWallActor;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API UWallPlacementMode : public UBuildingCreationSubMode {
	GENERATED_BODY()

public:
	UWallPlacementMode();

	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wall")
	TSubclassOf<AWallActor> WallActorRef;

private:
	UFUNCTION()
	void HandleLeftClickAction();

	UFUNCTION()
	void HandleRKeyPressAction();

	UFUNCTION()
	void HandleMKeyPressAction();

	UPROPERTY(VisibleDefaultsOnly, Category = "Wall")
	AWallActor* WallActor;

	bool bNewWallStart;
};
