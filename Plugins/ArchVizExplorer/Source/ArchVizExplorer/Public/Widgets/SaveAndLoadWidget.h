// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/BackgroundBlur.h"
#include "SaveAndLoadWidget.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API USaveAndLoadWidget : public UUserWidget {
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// Save and Load Menu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* LoadProjectMenuButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SaveProjectMenuButton;

	UFUNCTION()
	void HandleLoadMenuButtonClick();

	UFUNCTION()
	void HandleSaveMenuButtonClick();

	// Save Popup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* SavePopup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* CancelSaveButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SaveButton;

	UFUNCTION()
	void HandleCancelSaveButtonClick();

	// Load Popup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* LoadPopup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* CancelLoadButton;

	UFUNCTION()
	void HandleCancelLoadButtonClick();

	// BackGround Blur
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBackgroundBlur* BgBlur;

};
