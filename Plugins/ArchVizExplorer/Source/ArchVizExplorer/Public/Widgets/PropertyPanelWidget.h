// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/SpinBox.h"
#include "UMG/Public/Components/Button.h"
#include "PropertyPanelWidget.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UPropertyPanelWidget : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	// Wall Grid
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* WallLengthSpinBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* NewWallButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* DeleteWallButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* ClosePanelWallButton;

	// Door Grid
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* NewDoorButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* DeleteDoorButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* ClosePanelDoorButton;


	// Floor Grid
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* FloorLengthSpinBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* FloorWidthSpinBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* FloorHeightSpinBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* NewFloorButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* DeleteFloorButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* ClosePanelFloorButton;

	// Roof Grid
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* RoofLengthSpinBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* RoofWidthSpinBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* RoofHeightSpinBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* NewRoofButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* DeleteRoofButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* ClosePanelRoofButton;
};
