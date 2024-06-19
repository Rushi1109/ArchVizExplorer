// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BuildingCreationWidget.h"

void UBuildingCreationWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (IsValid(WallIMG)) {
		WallIMG->OnMouseButtonDownEvent.BindUFunction(this, "HandleWallEntityButtonClick");
	}
	if (IsValid(DoorIMG)) {
		DoorIMG->OnMouseButtonDownEvent.BindUFunction(this, "HandleDoorEntityButtonClick");
	}
	if (IsValid(FloorIMG)) {
		FloorIMG->OnMouseButtonDownEvent.BindUFunction(this, "HandleFloorEntityButtonClick");
	}
	if (IsValid(RoofIMG)) {
		RoofIMG->OnMouseButtonDownEvent.BindUFunction(this, "HandleRoofEntityButtonClick");
	}
}

void UBuildingCreationWidget::HandleWallEntityButtonClick(FGeometry, FPointerEvent&) {
	OnBuildingModeEntityChange.Broadcast(EBuildingModeEntity::WallPlacement);
}

void UBuildingCreationWidget::HandleDoorEntityButtonClick(FGeometry, FPointerEvent&) {
	OnBuildingModeEntityChange.Broadcast(EBuildingModeEntity::DoorPlacement);
}

void UBuildingCreationWidget::HandleFloorEntityButtonClick(FGeometry, FPointerEvent&) {
	OnBuildingModeEntityChange.Broadcast(EBuildingModeEntity::FloorPlacement);
}

void UBuildingCreationWidget::HandleRoofEntityButtonClick(FGeometry, FPointerEvent&) {
	OnBuildingModeEntityChange.Broadcast(EBuildingModeEntity::RoofPlacement);
}