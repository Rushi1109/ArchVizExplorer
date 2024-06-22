// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BuildingCreationWidget.h"

void UBuildingCreationWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (IsValid(WallButton)) {
		WallButton->OnClicked.AddDynamic(this, &UBuildingCreationWidget::HandleWallEntityButtonClick);
	}
	if (IsValid(DoorButton)) {
		DoorButton->OnClicked.AddDynamic(this, &UBuildingCreationWidget::HandleDoorEntityButtonClick);
	}
	if (IsValid(FloorButton)) {
		FloorButton->OnClicked.AddDynamic(this, &UBuildingCreationWidget::HandleFloorEntityButtonClick);
	}
	if (IsValid(RoofButton)) {
		RoofButton->OnClicked.AddDynamic(this, &UBuildingCreationWidget::HandleRoofEntityButtonClick);
	}
}

void UBuildingCreationWidget::HandleWallEntityButtonClick() {
	OnBuildingModeEntityChange.Broadcast(EBuildingModeEntity::WallPlacement);
}

void UBuildingCreationWidget::HandleDoorEntityButtonClick() {
	OnBuildingModeEntityChange.Broadcast(EBuildingModeEntity::DoorPlacement);
}

void UBuildingCreationWidget::HandleFloorEntityButtonClick() {
	OnBuildingModeEntityChange.Broadcast(EBuildingModeEntity::FloorPlacement);
}

void UBuildingCreationWidget::HandleRoofEntityButtonClick() {
	OnBuildingModeEntityChange.Broadcast(EBuildingModeEntity::RoofPlacement);
}