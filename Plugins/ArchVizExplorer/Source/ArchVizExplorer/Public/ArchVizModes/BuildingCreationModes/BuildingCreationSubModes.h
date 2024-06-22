// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingCreationSubModes.generated.h"

class APlayerController;
class UInputMappingContext;

/**
 *
 */
UCLASS(ABSTRACT)
class ARCHVIZEXPLORER_API UBuildingCreationSubMode : public UObject {
	GENERATED_BODY()
public:

	virtual void Setup() PURE_VIRTUAL(UBuildingCreationSubMode::Setup(), );
	virtual void SetupInputMapping() PURE_VIRTUAL(UBuildingCreationSubMode::SetupInputMapping(), );
	virtual void EnterSubMode() PURE_VIRTUAL(UBuildingCreationSubMode::EnterSubMode(), );
	virtual void ExitSubMode() PURE_VIRTUAL(UBuildingCreationSubMode::ExitSubMode(), );

	virtual void InitParams(APlayerController* Controller);

protected:
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;
};
