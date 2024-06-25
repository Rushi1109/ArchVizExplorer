// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingCreationSubModes.generated.h"

class APlayerController;
class UInputMappingContext;

UENUM(BlueprintType)
enum class EBuildingSubModeState : uint8 {
	Free,
	OldEntity,
	NewEntity
};

/**
 *
 */
UCLASS(ABSTRACT)
class ARCHVIZEXPLORER_API UBuildingCreationSubMode : public UObject {
	GENERATED_BODY()
public:

	virtual void Setup() PURE_VIRTUAL(UBuildingCreationSubMode::Setup(), );
	virtual void Cleanup() PURE_VIRTUAL(UBuildingCreationSubMode::Cleanup(), );
	virtual void SetupInputMapping() PURE_VIRTUAL(UBuildingCreationSubMode::SetupInputMapping(), );
	virtual void EnterSubMode() PURE_VIRTUAL(UBuildingCreationSubMode::EnterSubMode(), );
	virtual void ExitSubMode() PURE_VIRTUAL(UBuildingCreationSubMode::ExitSubMode(), );

	virtual void InitParams(APlayerController* Controller);

protected:
	FHitResult GetHitResult(const TArray<AActor*>& ActorsToIgnore = TArray<AActor*>{}) const;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;

	EBuildingSubModeState SubModeState;
};
