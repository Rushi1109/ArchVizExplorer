// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationMode.h"

UBuildingCreationMode::UBuildingCreationMode() : CurrentBuildingCreationSubMode{ nullptr }, PlayerController{nullptr}, InputMappingContext{nullptr} {}

void UBuildingCreationMode::SetupSubModes() {
	if (IsValid(WallPlacementModeRef)) {
		WallPlacementMode = NewObject<UWallPlacementMode>(this, WallPlacementModeRef);
		WallPlacementMode->InitParam(PlayerController);
	}
}

void UBuildingCreationMode::InitParam(APlayerController* Controller) {
	PlayerController = Controller;
}

void UBuildingCreationMode::SetSubMode(IBuildingCreationMode* NewSubMode) {
	if (CurrentBuildingCreationSubMode) {
		CurrentBuildingCreationSubMode->ExitSubMode();
	}

	CurrentBuildingCreationSubMode = NewSubMode;

	if (CurrentBuildingCreationSubMode) {
		CurrentBuildingCreationSubMode->EnterSubMode();
	}
}

void UBuildingCreationMode::SetBuildingModeEntity(EBuildingModeEntity NewEntity) {
	if (CurrentBuildingCreationSubMode) {
		CurrentBuildingCreationSubMode->CleanUp();
	}

	BuildingModeEntity = NewEntity;

	switch (BuildingModeEntity) {
	case EBuildingModeEntity::None:
		CurrentBuildingCreationSubMode = nullptr;
		break;
	case EBuildingModeEntity::WallPlacement:
		SetSubMode(WallPlacementMode);
		break;
	case EBuildingModeEntity::DoorPlacement:
		CurrentBuildingCreationSubMode = nullptr;
		break;
	case EBuildingModeEntity::FloorPlacement:
		break;
	case EBuildingModeEntity::RoofPlacement:
		break;
	}
}

void UBuildingCreationMode::PreviewSegment() {
	if(CurrentBuildingCreationSubMode) {
		CurrentBuildingCreationSubMode->PreviewSegment();
	}
}

void UBuildingCreationMode::SetupInputMapping() {
	if (IsValid(WallPlacementMode)) {
		WallPlacementMode->SetupInputMapping();
	}
}

void UBuildingCreationMode::EnterMode() {

}

void UBuildingCreationMode::ExitMode() {

}