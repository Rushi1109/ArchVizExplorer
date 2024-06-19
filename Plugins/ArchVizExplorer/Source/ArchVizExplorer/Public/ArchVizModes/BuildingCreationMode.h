// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizMode.h"
#include "UObject/NoExportTypes.h"
#include "Enums/BuildingModeEntityEnum.h"
#include "InputMappingContext.h"
#include "BuildingCreationMode.generated.h"

class AWallActor;
class APlayerController;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API UBuildingCreationMode : public UObject, public IArchVizMode {
	GENERATED_BODY()

public:
	UBuildingCreationMode();

	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;

	void InitParam(APlayerController* Controller);

	void SetBuildingModeEntity(EBuildingModeEntity Entity);

private:
	UPROPERTY()
	EBuildingModeEntity BuildingModeEntity;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;
};
