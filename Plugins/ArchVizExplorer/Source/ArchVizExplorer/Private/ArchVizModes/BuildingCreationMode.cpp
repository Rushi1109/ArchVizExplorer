// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationMode.h"

UBuildingCreationMode::UBuildingCreationMode() : CurrentBuildingCreationSubMode{ nullptr }, BuildingModeEntity{ EBuildingModeEntity::None } {}

void UBuildingCreationMode::Setup() {
	if (IsValid(WallPlacementModeRef)) {
		WallPlacementMode = NewObject<UWallPlacementMode>(this, WallPlacementModeRef);
		WallPlacementMode->InitParams(PlayerController);
		WallPlacementMode->Setup();
	}
	if (IsValid(DoorPlacementModeRef)) {
		DoorPlacementMode = NewObject<UDoorPlacementMode>(this, DoorPlacementModeRef);
		DoorPlacementMode->InitParams(PlayerController);
		DoorPlacementMode->Setup();
	}
	if (IsValid(FloorPlacementModeRef)) {
		FloorPlacementMode = NewObject<UFloorPlacementMode>(this, FloorPlacementModeRef);
		FloorPlacementMode->InitParams(PlayerController);
		FloorPlacementMode->Setup();
	}
	if (IsValid(RoofPlacementModeRef)) {
		RoofPlacementMode = NewObject<URoofPlacementMode>(this, RoofPlacementModeRef);
		RoofPlacementMode->InitParams(PlayerController);
		RoofPlacementMode->Setup();
	}

	if (IsValid(WidgetRef)) {
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetRef, TEXT("Building Creation Widget"));
		if(auto* BuildingCreationWidget = Cast<UBuildingCreationWidget>(Widget)) {
			BuildingCreationWidget->OnBuildingModeEntityChange.AddUObject(this, &UBuildingCreationMode::HandleBuildingSubModeChange);
		}
	}
}

void UBuildingCreationMode::SetSubMode(UBuildingCreationSubMode* NewSubMode) {
	if (CurrentBuildingCreationSubMode) {
		CurrentBuildingCreationSubMode->ExitSubMode();
	}

	CurrentBuildingCreationSubMode = NewSubMode;

	if (CurrentBuildingCreationSubMode) {
		CurrentBuildingCreationSubMode->EnterSubMode();
	}
}

void UBuildingCreationMode::HandleBuildingSubModeChange(EBuildingModeEntity NewBuildingModeEntity) {
	BuildingModeEntity = NewBuildingModeEntity;

	UpdateBuildingModeEntity();
}

void UBuildingCreationMode::UpdateBuildingModeEntity() {
	switch (BuildingModeEntity) {
	case EBuildingModeEntity::None:
		CurrentBuildingCreationSubMode = nullptr;
		break;
	case EBuildingModeEntity::WallPlacement:
		SetSubMode(WallPlacementMode);
		break;
	case EBuildingModeEntity::DoorPlacement:
		SetSubMode(DoorPlacementMode);
		break;
	case EBuildingModeEntity::FloorPlacement:
		SetSubMode(FloorPlacementMode);
		break;
	case EBuildingModeEntity::RoofPlacement:
		SetSubMode(RoofPlacementMode);
		break;
	}
}

void UBuildingCreationMode::SetupInputMapping() {
	if (IsValid(WallPlacementMode)) {
		WallPlacementMode->SetupInputMapping();
	}
	if (IsValid(DoorPlacementMode)) {
		DoorPlacementMode->SetupInputMapping();
	}
	if (IsValid(FloorPlacementMode)) {
		FloorPlacementMode->SetupInputMapping();
	}
	if (IsValid(RoofPlacementMode)) {
		RoofPlacementMode->SetupInputMapping();
	}
}

void UBuildingCreationMode::EnterMode() {
	ShowWidget();
}

void UBuildingCreationMode::ExitMode() {
	if (CurrentBuildingCreationSubMode) {
		CurrentBuildingCreationSubMode->ExitSubMode();
	}
	HideWidget();
}