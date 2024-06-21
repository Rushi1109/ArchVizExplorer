// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class ARCHVIZEXPLORER_API ArchVizUtility {
public:
	ArchVizUtility();
	~ArchVizUtility();

	static FVector SnapToGrid(const FVector& WorldLocation, const FVector& GridSize = FVector{100.0});
};
