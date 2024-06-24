// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ArchVizActor.h"
#include "BuildingCreationActor.generated.h"

UENUM(BlueprintType)
enum class EBuildingActorState : uint8 {
	Selected,
	Previewing,
	Generating,
	Moving
};

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API ABuildingCreationActor : public AArchVizActor {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArchVizActor | PropertyWidget")
	TSubclassOf<UUserWidget> PropertyPanelRef;

	EBuildingActorState GetState();
	void SetState(EBuildingActorState NewState);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ArchVizActor | PropertyWidget")
	UUserWidget* PropertyPanel;

	EBuildingActorState State;

	void ShowPropertyPanel();
	void HidePropertyPanel();

	void RotateActor(double Degrees);
};
