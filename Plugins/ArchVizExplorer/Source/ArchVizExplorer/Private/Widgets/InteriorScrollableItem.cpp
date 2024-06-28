// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteriorScrollableItem.h"

void UInteriorScrollableItem::NativeConstruct() {
	Super::NativeConstruct();

	if (IsValid(ItemButton)) {
		ItemButton->OnClicked.AddDynamic(this, &UInteriorScrollableItem::HandleItemButtonClick);
	}
}

void UInteriorScrollableItem::HandleItemButtonClick() {
	OnItemButtonClicked.ExecuteIfBound(InteriorAsset);
}

void UInteriorScrollableItem::SetItemData(const FInteriorAsset& NewInteriorAsset) {
	InteriorAsset = NewInteriorAsset;

	if (IsValid(InteriorAsset.Thumbnail)) {
		ItemThumbnail->SetBrushResourceObject(InteriorAsset.Thumbnail);
	}
}
