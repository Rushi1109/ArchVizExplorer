// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizMode.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "RoadConstructionMode.generated.h"

class ARoadActor;
class APlayerController;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API URoadConstructionMode : public UObject, public IArchVizMode {
	GENERATED_BODY()

public:
	URoadConstructionMode();

	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;

	void InitParam(APlayerController* Controller);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Road")
	TSubclassOf<ARoadActor> RoadActorRef;

private:
	UFUNCTION()
	void HandleLeftClickAction();

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(VisibleDefaultsOnly, Category = "Road")
	ARoadActor* RoadActor;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;
};
