// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/BackgroundBlur.h"
#include "UMG/Public/Components/EditableText.h"
#include "SaveSlotItem.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "SaveAndLoadWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnSaveSlotReceived, const FString&)
DECLARE_DELEGATE_OneParam(FOnSaveSlotDeleteReceived, const FString&)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API USaveAndLoadWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveSlotItemRef")
	TSubclassOf<USaveSlotItem> SaveSlotItemRef;

public:
	FOnSaveSlotReceived OnSaveSlotReceived;
	FOnSaveSlotDeleteReceived OnSaveSlotDeleteReceived;

	void PopulateSavedSlots(TArray<FString> SaveSlots);

	virtual void NativeConstruct() override;

	// Save and Load Menu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* NewProjectMenuButton;

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
	UEditableText* SaveSlotName;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* SaveSlotScrollBox;

	UFUNCTION()
	void HandleCancelLoadButtonClick();
	// BackGround Blur
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBackgroundBlur* BgBlur;

	void ShowLoadPopup();
	void HideLoadPopup();
	void ShowSavePopup();
	void HideSavePopup();
};
