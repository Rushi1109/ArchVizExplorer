// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ArchVizActor.h"
#include "BuildingCreationActor.generated.h"

UENUM(BlueprintType)
enum class EBuildingActorState : uint8 {
	None,
	Selected,
	Previewing,
	Generating,
	Moving
};

class UPropertyPanelWidget;

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API ABuildingCreationActor : public AArchVizActor {
	GENERATED_BODY()

public:
	ABuildingCreationActor();

	EBuildingActorState GetState();
	void SetState(EBuildingActorState NewState);
	void HandleStateChange();


protected:
	EBuildingActorState State;
};
