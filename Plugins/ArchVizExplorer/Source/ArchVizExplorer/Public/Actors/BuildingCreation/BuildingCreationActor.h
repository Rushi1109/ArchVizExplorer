// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ArchVizActor.h"
#include "BuildingCreationActor.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API ABuildingCreationActor : public AArchVizActor {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArchVizActor | PropertyWidget")
	TSubclassOf<UUserWidget> PropertyPanelRef;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ArchVizActor | PropertyWidget")
	UUserWidget* PropertyPanel;

	void RotateActorBy90Degrees();
};
