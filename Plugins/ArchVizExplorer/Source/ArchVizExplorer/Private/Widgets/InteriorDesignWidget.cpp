// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteriorDesignWidget.h"

void UInteriorDesignWidget::NativeConstruct() {
	Super::NativeConstruct();

	if(IsValid(ChairButton)) {
		ChairButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleChairButtonClick);
	}
	if (IsValid(TableButton)) {
		TableButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleTableButtonClick);
	}
	if (IsValid(BedButton)) {
		BedButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleBedButtonClick);
	}
	if (IsValid(CeilingLightButton)) {
		CeilingLightButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleCeilingLightButtonClick);
	}
	if (IsValid(LampButton)) {
		LampButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleLampButtonClick);
	}
	if (IsValid(FrameButton)) {
		FrameButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleFrameButtonClick);
	}
}

void UInteriorDesignWidget::HandleChairButtonClick() {
	if (IsValid(InteriorScrollBox) && IsValid(ChairDataAsset)) {
		InteriorScrollBox->PopulateWidget(ChairDataAsset);
	}
}

void UInteriorDesignWidget::HandleTableButtonClick() {
	if (IsValid(InteriorScrollBox) && IsValid(TableDataAsset)) {
		InteriorScrollBox->PopulateWidget(TableDataAsset);
	}
}

void UInteriorDesignWidget::HandleBedButtonClick() {
	if (IsValid(InteriorScrollBox) && IsValid(BedDataAsset)) {
		InteriorScrollBox->PopulateWidget(BedDataAsset);
	}
}

void UInteriorDesignWidget::HandleCeilingLightButtonClick() {
	if (IsValid(InteriorScrollBox) && IsValid(CeilingLightDataAsset)) {
		InteriorScrollBox->PopulateWidget(CeilingLightDataAsset);
	}
}

void UInteriorDesignWidget::HandleLampButtonClick() {
	if (IsValid(InteriorScrollBox) && IsValid(LampDataAsset)) {
		InteriorScrollBox->PopulateWidget(LampDataAsset);
	}
}

void UInteriorDesignWidget::HandleFrameButtonClick() {
	if (IsValid(InteriorScrollBox) && IsValid(FrameDataAsset)) {
		InteriorScrollBox->PopulateWidget(FrameDataAsset);
	}
}