// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MaterialItem.h"

void UMaterialItem::SetItemData(const FMaterialAsset& NewItemData) {
	ItemData = NewItemData;

	if (IsValid(ItemData.Thumbnail)) {
		ItemImage->SetBrushResourceObject(ItemData.Thumbnail);
	}
}

void UMaterialItem::NativeOnInitialized() {
	Super::NativeOnInitialized();

	if (IsValid(ItemButton)) {
		ItemButton->OnClicked.AddDynamic(this, &UMaterialItem::HandleItemButtonClick);
	}
}

void UMaterialItem::HandleItemButtonClick() {
	OnItemButtonClicked.ExecuteIfBound(ItemData);
}
