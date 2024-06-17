// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArchVizModeEnum.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Button.h"
#include "ModesMenuWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnArchVizModeChanged, EArchVizMode);

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UModesMenuWidget : public UUserWidget {
	GENERATED_BODY()
public:
	FOnArchVizModeChanged OnModeChanged;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleShowModesButtonClick();

	UFUNCTION()
	void HandleRoadModeButtonClick();

	UFUNCTION()
	void HandleBuildingModeButtonClick();

	UFUNCTION()
	void HandleInteriorModeButtonClick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Canvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextShowMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ShowModesButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* RoadConstructionButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BuildingCreationButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* InteriorDesignButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowOptionMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HideOptionMenu;

	UPROPERTY()
	bool bToggleModeMenu;
};
