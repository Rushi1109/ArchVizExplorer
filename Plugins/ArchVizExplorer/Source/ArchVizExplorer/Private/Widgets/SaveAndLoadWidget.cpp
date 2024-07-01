// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SaveAndLoadWidget.h"

void USaveAndLoadWidget::NativeConstruct() {
	Super::NativeConstruct();


	if (IsValid(LoadProjectMenuButton)) {
		LoadProjectMenuButton->OnClicked.AddDynamic(this, &USaveAndLoadWidget::HandleLoadMenuButtonClick);
	}

	if (IsValid(SaveProjectMenuButton)) {
		SaveProjectMenuButton->OnClicked.AddDynamic(this, &USaveAndLoadWidget::HandleSaveMenuButtonClick);
	}

	if (IsValid(CancelSaveButton)) {
		CancelSaveButton->OnClicked.AddDynamic(this, &USaveAndLoadWidget::HandleCancelSaveButtonClick);
	}

	if (IsValid(CancelLoadButton)) {
		CancelLoadButton->OnClicked.AddDynamic(this, &USaveAndLoadWidget::HandleCancelLoadButtonClick);
	}
}


void USaveAndLoadWidget::HandleLoadMenuButtonClick() {
	LoadPopup->SetVisibility(ESlateVisibility::Visible);
	BgBlur->SetIsEnabled(true);
	BgBlur->SetBlurStrength(2.0f);
}

void USaveAndLoadWidget::HandleSaveMenuButtonClick() {
	SavePopup->SetVisibility(ESlateVisibility::Visible);
	BgBlur->SetIsEnabled(true);
	BgBlur->SetBlurStrength(2.0f);
}

void USaveAndLoadWidget::HandleCancelSaveButtonClick() {
	SavePopup->SetVisibility(ESlateVisibility::Hidden);
	BgBlur->SetIsEnabled(false);
	BgBlur->SetBlurStrength(0.0f);
}

void USaveAndLoadWidget::HandleCancelLoadButtonClick() {
	LoadPopup->SetVisibility(ESlateVisibility::Hidden);
	BgBlur->SetIsEnabled(false);
	BgBlur->SetBlurStrength(0.0f);
}
