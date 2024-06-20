// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingCreationModes.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "WallPlacementMode.generated.h"

class AWallActor;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API UWallPlacementMode : public UObject, public IBuildingCreationMode {
	GENERATED_BODY()

public:
	UWallPlacementMode();

	virtual void SetupInputMapping() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;

	void InitParam(APlayerController* Controller);

	virtual void PreviewSegment() override;
	virtual void CleanUp() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wall")
	TSubclassOf<AWallActor> WallActorRef;

private:
	UFUNCTION()
	void HandleLeftClickAction();

	UFUNCTION()
	void HandleRKeyPressAction();

	FHitResult GetHitResult() const;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;

	UPROPERTY(VisibleDefaultsOnly, Category = "Wall")
	AWallActor* WallActor;
};
