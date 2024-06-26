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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArchVizActor | PropertyWidget")
	TSubclassOf<UPropertyPanelWidget> PropertyPanelRef;

	EBuildingActorState GetState();
	void SetState(EBuildingActorState NewState);
	void HandleStateChange();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ArchVizActor | PropertyWidget")
	UPropertyPanelWidget* PropertyPanel;

	EBuildingActorState State;

	void ShowPropertyPanel();
	void HidePropertyPanel();

	void RotateActor(double Degrees);
};
