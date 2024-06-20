// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class ARCHVIZEXPLORER_API IBuildingCreationMode {
public:
	virtual ~IBuildingCreationMode() = default;

	virtual void SetupInputMapping() = 0;
	virtual void EnterSubMode() = 0;
	virtual void ExitSubMode() = 0;

	virtual void PreviewSegment() = 0;
	virtual void CleanUp() = 0;
};
