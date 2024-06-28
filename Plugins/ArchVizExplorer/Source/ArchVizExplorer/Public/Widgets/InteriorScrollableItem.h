// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Image.h"
#include "DataAssets/InteriorDataAsset.h"
#include "InteriorScrollableItem.generated.h"

DECLARE_DELEGATE_OneParam(FOnItemButtonClicked, FInteriorAsset)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UInteriorScrollableItem : public UUserWidget {
	GENERATED_BODY()

public:
	void SetItemData(const FInteriorAsset& NewInteriorAsset);

	FOnItemButtonClicked OnItemButtonClicked;
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemThumbnail;

	virtual void NativeConstruct() override;

private:
	FInteriorAsset InteriorAsset;
 
	UFUNCTION()
	void HandleItemButtonClick();
};
