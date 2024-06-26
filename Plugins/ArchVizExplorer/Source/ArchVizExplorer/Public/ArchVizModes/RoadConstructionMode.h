// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizMode.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "Widgets/RoadConstructionWidget.h"
#include "RoadConstructionMode.generated.h"

class ARoadActor;
class APlayerController;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API URoadConstructionMode : public UArchVizMode {
	GENERATED_BODY()

public:
	URoadConstructionMode();

	virtual void Setup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RoadConstrcution | Actor")
	TSubclassOf<ARoadActor> RoadActorRef;

private:
	UFUNCTION()
	void HandleLeftClickAction();

	ARoadActor* RoadActor;
};
