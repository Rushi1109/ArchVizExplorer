// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MaterialScrollBox.h"
#include "UMG/Public/Components/ScrollBoxSlot.h"

void UMaterialScrollBox::NativeConstruct() {
	Super::NativeConstruct();
}

void UMaterialScrollBox::PopulateWidget(UMaterialDataAsset* DataAsset) {
	if (IsValid(DataAsset) && IsValid(ScrollBox)) {
		ScrollBox->ClearChildren();

		TArray<FMaterialAsset> Assets = DataAsset->MaterialAssets;

		for (auto& Asset : Assets) {
			if (MaterialItemWidgetRef) {
				if (auto* ItemWidget = CreateWidget<UMaterialItem>(this, MaterialItemWidgetRef)) {
					ItemWidget->SetItemData(Asset);

					ItemWidget->OnItemButtonClicked.BindLambda(
						[this](FMaterialAsset Asset) {
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
