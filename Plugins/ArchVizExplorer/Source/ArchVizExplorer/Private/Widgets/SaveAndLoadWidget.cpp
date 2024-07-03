// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SaveAndLoadWidget.h"
#include "UMG/Public/Components/ScrollBoxSlot.h"

void USaveAndLoadWidget::PopulateSavedSlots(TArray<FString> SaveSlots) {
	if (IsValid(SaveSlotScrollBox)) {
		SaveSlotScrollBox->ClearChildren();

		for (const auto& SaveSlot : SaveSlots) {
			if (SaveSlotItemRef) {
				if (USaveSlotItem* ItemWidget = CreateWidget<USaveSlotItem>(this, SaveSlotItemRef)) {
					ItemWidget->SetItemData(SaveSlot);
					ItemWidget->OnItemSelectButtonClicked.BindLambda(
						[this](const FString& SlotName) {
							OnSaveSlotReceived.ExecuteIfBound(SlotName);
						});

					ItemWidget->OnItemDeleteButtonClicked.BindLambda(
						[this](const FString& SlotName) {
							OnSaveSlotDeleteReceived.ExecuteIfBound(SlotName);
						});

					auto* PanelSlot = SaveSlotScrollBox->AddChild(ItemWidget);
					if (auto* ScrollBoxSlot = Cast<UScrollBoxSlot>(PanelSlot)) {
						ScrollBoxSlot->SetPadding(FMargin{ 6.0 });
					}
				}
			}
		}
	}
}

void USaveAndLoadWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();

	if (IsValid(CancelSaveButton)) {
		CancelSaveButton->OnClicked.AddDynamic(this, &USaveAndLoadWidget::HandleCancelSaveButtonClick);
	}

	if (IsValid(CancelLoadButton)) {
		CancelLoadButton->OnClicked.AddDynamic(this, &USaveAndLoadWidget::HandleCancelLoadButtonClick);
	}
}

void USaveAndLoadWidget::HandleCancelSaveButtonClick() {
	HideSavePopup();
}

void USaveAndLoadWidget::HandleCancelLoadButtonClick() {
	HideLoadPopup();
}

void USaveAndLoadWidget::ShowLoadPopup() {
	LoadPopup->SetVisibility(ESlateVisibility::Visible);
	BgBlur->SetIsEnabled(true);
	BgBlur->SetBlurStrength(2.0f);
}

void USaveAndLoadWidget::HideLoadPopup() {
	LoadPopup->SetVisibility(ESlateVisibility::Hidden);
	BgBlur->SetIsEnabled(false);
	BgBlur->SetBlurStrength(0.0f);
}

void USaveAndLoadWidget::ShowSavePopup() {
	SavePopup->SetVisibility(ESlateVisibility::Visible);
	BgBlur->SetIsEnabled(true);
	BgBlur->SetBlurStrength(2.0f);
}

void USaveAndLoadWidget::HideSavePopup() {
	SavePopup->SetVisibility(ESlateVisibility::Hidden);
	BgBlur->SetIsEnabled(false);
	BgBlur->SetBlurStrength(0.0f);
}

void USaveAndLoadWidget::HideAllPopup() {
	HideSavePopup();
	HideLoadPopup();
}
