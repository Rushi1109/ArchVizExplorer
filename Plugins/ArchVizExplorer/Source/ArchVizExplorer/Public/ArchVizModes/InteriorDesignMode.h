// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizModes/ArchVizMode.h"
#include "InputMappingContext.h"
#include "UObject/NoExportTypes.h"
#include "InteriorDesignMode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API UInteriorDesignMode : public UArchVizMode {
	GENERATED_BODY()

public:
	UInteriorDesignMode();

	virtual void Setup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;

private:
	UFUNCTION()
	void HandleLeftClickAction();
};
