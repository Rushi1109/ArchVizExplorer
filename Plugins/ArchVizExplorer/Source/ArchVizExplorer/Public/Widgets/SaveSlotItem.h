// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "SaveSlotItem.generated.h"

DECLARE_DELEGATE_OneParam(FOnSaveSlotSelected, const FString&)
DECLARE_DELEGATE_OneParam(FOnSaveSlotDeleted, const FString&)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API USaveSlotItem : public UUserWidget {
	GENERATED_BODY()

public:
	void SetItemData(const FString& InSlotName);

	FOnSaveSlotSelected OnItemSelectButtonClicked;
	FOnSaveSlotDeleted OnItemDeleteButtonClicked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* ItemDeleteButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UImage* DeleteIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* ItemNameButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SlotNameTextBox;

	virtual void NativeConstruct() override;

private:
	FString SlotName;
 
	UFUNCTION()
	void HandleItemDeleteButtonClick();

	UFUNCTION()
	void HandleItemNameButtonClick();
};