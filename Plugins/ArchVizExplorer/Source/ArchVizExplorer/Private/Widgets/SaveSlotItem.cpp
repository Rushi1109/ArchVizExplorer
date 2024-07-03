// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SaveSlotItem.h"

void USaveSlotItem::NativeOnInitialized() {
	Super::NativeOnInitialized();

	if (IsValid(ItemDeleteButton)) {
		ItemDeleteButton->OnClicked.AddDynamic(this, &USaveSlotItem::HandleItemDeleteButtonClick);
	}

	if (IsValid(ItemNameButton)) {
		ItemNameButton->OnClicked.AddDynamic(this, &USaveSlotItem::HandleItemNameButtonClick);
	}
}

void USaveSlotItem::HandleItemDeleteButtonClick() {
	OnItemDeleteButtonClicked.ExecuteIfBound(SlotName);
}

void USaveSlotItem::HandleItemNameButtonClick() {
	OnItemSelectButtonClicked.ExecuteIfBound(SlotName);
}

void USaveSlotItem::SetItemData(const FString& InSlotName) {
	SlotName = InSlotName;

	SlotNameTextBox->SetText(FText::FromString(SlotName));
}
