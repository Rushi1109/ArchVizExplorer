// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingCreationModes.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "FloorPlacementMode.generated.h"

class AFloorActor;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API UFloorPlacementMode : public UObject, public IBuildingCreationMode {
	GENERATED_BODY()

public:
	UFloorPlacementMode();

	virtual void SetupInputMapping() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;

	void InitParam(APlayerController* Controller);

	virtual void PreviewSegment() override;
	virtual void CleanUp() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wall")
	TSubclassOf<AFloorActor> FloorActorRef;

private:
	UFUNCTION()
	void HandleLeftClickAction();

	FHitResult GetHitResult() const;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;

	UPROPERTY(VisibleDefaultsOnly, Category = "Wall")
	AFloorActor* FloorActor;

	bool bIsDragging;
};
