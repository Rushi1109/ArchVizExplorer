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

void UBuildingCreationWidget::HighlightSelectedColour(EBuildingModeEntity NewBuildingModeEntity) {
	switch (NewBuildingModeEntity) {
	case EBuildingModeEntity::WallPlacement:
		if (IsValid(WallButton)) {
			FButtonStyle ButtonStyle = WallButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			WallButton->SetStyle(ButtonStyle);
		}
		if (IsValid(DoorButton)) {
			FButtonStyle ButtonStyle = DoorButton->GetStyle();
			SetNormalTint(ButtonStyle);
			DoorButton->SetStyle(ButtonStyle);
		}
		if (IsValid(FloorButton)) {
			FButtonStyle ButtonStyle = FloorButton->GetStyle();
			SetNormalTint(ButtonStyle);
			FloorButton->SetStyle(ButtonStyle);
		}
		if (IsValid(RoofButton)) {
			FButtonStyle ButtonStyle = RoofButton->GetStyle();
			SetNormalTint(ButtonStyle);
			RoofButton->SetStyle(ButtonStyle);
		}
		break;
	case EBuildingModeEntity::DoorPlacement:
		if (IsValid(WallButton)) {
			FButtonStyle ButtonStyle = WallButton->GetStyle();
			SetNormalTint(ButtonStyle);
			WallButton->SetStyle(ButtonStyle);
		}
		if (IsValid(DoorButton)) {
			FButtonStyle ButtonStyle = DoorButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			DoorButton->SetStyle(ButtonStyle);
		}
		if (IsValid(FloorButton)) {
			FButtonStyle ButtonStyle = FloorButton->GetStyle();
			SetNormalTint(ButtonStyle);
			FloorButton->SetStyle(ButtonStyle);
		}
		if (IsValid(RoofButton)) {
			FButtonStyle ButtonStyle = RoofButton->GetStyle();
			SetNormalTint(ButtonStyle);
			RoofButton->SetStyle(ButtonStyle);
		}
		break;
	case EBuildingModeEntity::FloorPlacement:
		if (IsValid(WallButton)) {
			FButtonStyle ButtonStyle = WallButton->GetStyle();
			SetNormalTint(ButtonStyle);
			WallButton->SetStyle(ButtonStyle);
		}
		if (IsValid(DoorButton)) {
			FButtonStyle ButtonStyle = DoorButton->GetStyle();
			SetNormalTint(ButtonStyle);
			DoorButton->SetStyle(ButtonStyle);
		}
		if (IsValid(FloorButton)) {
			FButtonStyle ButtonStyle = FloorButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			FloorButton->SetStyle(ButtonStyle);
		}
		if (IsValid(RoofButton)) {
			FButtonStyle ButtonStyle = RoofButton->GetStyle();
			SetNormalTint(ButtonStyle);
			RoofButton->SetStyle(ButtonStyle);
		}
		break;
	case EBuildingModeEntity::RoofPlacement:
		if (IsValid(WallButton)) {
			FButtonStyle ButtonStyle = WallButton->GetStyle();
			SetNormalTint(ButtonStyle);
			WallButton->SetStyle(ButtonStyle);
		}
		if (IsValid(DoorButton)) {
			FButtonStyle ButtonStyle = DoorButton->GetStyle();
			SetNormalTint(ButtonStyle);
			DoorButton->SetStyle(ButtonStyle);
		}
		if (IsValid(FloorButton)) {
			FButtonStyle ButtonStyle = FloorButton->GetStyle();
			SetNormalTint(ButtonStyle);
			FloorButton->SetStyle(ButtonStyle);
		}
		if (IsValid(RoofButton)) {
			FButtonStyle ButtonStyle = RoofButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			RoofButton->SetStyle(ButtonStyle);
		}
		break;
	}
}


void UBuildingCreationWidget::SetNormalTint(FButtonStyle& ButtonStyle) {
	FSlateColor NewNormalTint = FSlateColor(FLinearColor(1.f, 1.f, 1.f));
	ButtonStyle.Normal.TintColor = NewNormalTint;
}

void UBuildingCreationWidget::SetHighlightedTint(FButtonStyle& ButtonStyle) {
	FSlateColor NewNormalTint = FSlateColor(FLinearColor(0.3125f, 0.3125f, 0.3125f));
	ButtonStyle.Normal.TintColor = NewNormalTint;
}
