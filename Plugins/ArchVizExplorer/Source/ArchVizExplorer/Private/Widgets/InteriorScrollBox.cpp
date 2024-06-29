// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteriorScrollBox.h"
#include "UMG/Public/Components/ScrollBoxSlot.h"

void UInteriorScrollBox::PopulateWidget(UInteriorDataAsset* DataAsset) {
	if (IsValid(DataAsset) && IsValid(ScrollBox)) {
		ScrollBox->ClearChildren();
		
		TArray<FInteriorAsset> InteriorAssets = DataAsset->InteriorAssets;

		for (const auto& Asset : InteriorAssets) {
			if (InteriorScrollableItemRef) {
				if (UInteriorScrollableItem* ItemWidget = CreateWidget<UInteriorScrollableItem>(this, InteriorScrollableItemRef)) {
					ItemWidget->SetItemData(Asset);
					ItemWidget->OnItemButtonClicked.BindLambda(
						[this](FInteriorAsset Asset) {
							OnItemSelected.ExecuteIfBound(Asset);
						});

					auto* PanelSlot = ScrollBox->AddChild(ItemWidget);
					if (auto* ScrollBoxSlot = Cast<UScrollBoxSlot>(PanelSlot)) {
						ScrollBoxSlot->SetPadding(FMargin{ 6.0 });
					}
				}
			}
		}
	}
}
