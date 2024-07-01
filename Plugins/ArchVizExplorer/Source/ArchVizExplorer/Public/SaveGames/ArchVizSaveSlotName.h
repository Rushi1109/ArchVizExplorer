// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ArchVizSaveSlotName.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UArchVizSaveSlotName : public USaveGame {
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FString> SlotsName;
};
