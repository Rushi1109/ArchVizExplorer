// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ModesMenuWidget.h"

void UModesMenuWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();

	bToggleModeMenu = true;

	if (IsValid(ShowModesButton)) {
		ShowModesButton->OnClicked.AddDynamic(this, &UModesMenuWidget::HandleShowModesButtonClick);
	}

	if (IsValid(RoadConstructionButton)) {
		RoadConstructionButton->OnClicked.AddDynamic(this, &UModesMenuWidget::HandleRoadModeButtonClick);
	}
	if (IsValid(BuildingCreationButton)) {
		BuildingCreationButton->OnClicked.AddDynamic(this, &UModesMenuWidget::HandleBuildingModeButtonClick);
	}
	if (IsValid(InteriorDesignButton)) {
		InteriorDesignButton->OnClicked.AddDynamic(this, &UModesMenuWidget::HandleInteriorModeButtonClick);
	}
	if (IsValid(SaveAndLoadButton)) {
		SaveAndLoadButton->OnClicked.AddDynamic(this, &UModesMenuWidget::HandleSaveAndLoadModeButtonClick);
	}
}

void UModesMenuWidget::HandleShowModesButtonClick() {
	if (bToggleModeMenu) {
		PlayAnimation(ShowOptionMenu, 0.0, 1, EUMGSequencePlayMode::Forward, 1.0, false);
		TextShowMenu->SetText(FText::FromString("<"));
	}
	else {
		PlayAnimation(HideOptionMenu, 0.0, 1, EUMGSequencePlayMode::Forward, 1.0, false);
		TextShowMenu->SetText(FText::FromString(">"));
	}
	bToggleModeMenu = !bToggleModeMenu;
}

void UModesMenuWidget::HandleRoadModeButtonClick() {
	OnModeChanged.Broadcast(EArchVizMode::RoadConstruction);
}

void UModesMenuWidget::HandleBuildingModeButtonClick() {
	OnModeChanged.Broadcast(EArchVizMode::BuildingCreation);
}

void UModesMenuWidget::HandleInteriorModeButtonClick() {
	OnModeChanged.Broadcast(EArchVizMode::InteriorDesign);
}

void UModesMenuWidget::HandleSaveAndLoadModeButtonClick() {
	OnModeChanged.Broadcast(EArchVizMode::SaveAndLoad);
}