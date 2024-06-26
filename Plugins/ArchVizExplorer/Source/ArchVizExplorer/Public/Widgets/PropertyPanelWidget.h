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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* WallLengthSpinBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* NewWallButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* DeleteWallButton;
};
