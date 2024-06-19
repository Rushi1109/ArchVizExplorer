// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */

class ARCHVIZEXPLORER_API IArchVizMode {
public:
	virtual ~IArchVizMode() = default;

	virtual void SetupInputMapping() = 0;
	virtual void EnterMode() = 0;
	virtual void ExitMode() = 0;
};
