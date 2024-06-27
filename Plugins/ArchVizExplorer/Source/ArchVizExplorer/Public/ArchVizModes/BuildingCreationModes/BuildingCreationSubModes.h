// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/BuildingModeEntityEnum.h"
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

DECLARE_DELEGATE_TwoParams(FOnOtherBuildingActorSelected, EBuildingModeEntity, AActor*)

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

	FOnOtherBuildingActorSelected OnOtherBuildingActorSelected;

protected:
	virtual void HandleFreeState() PURE_VIRTUAL(UBuildingCreationSubMode::HandleFreeState(), );
	virtual void HandleOldEntityState() PURE_VIRTUAL(UBuildingCreationSubMode::HandleOldEntityState(), );
	virtual void HandleNewEntityState() PURE_VIRTUAL(UBuildingCreationSubMode::HandleNewEntityState(), );

	FHitResult GetHitResult(const TArray<AActor*>& ActorsToIgnore = TArray<AActor*>{}) const;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;

	EBuildingSubModeState SubModeState;
};
