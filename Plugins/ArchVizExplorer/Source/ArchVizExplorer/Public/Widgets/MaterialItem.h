// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Image.h"
#include "DataAssets/MaterialDataAsset.h"
#include "MaterialItem.generated.h"

DECLARE_DELEGATE_OneParam(FOnMaterialItemButtonClicked, FMaterialAsset)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UMaterialItem : public UUserWidget {
	GENERATED_BODY()

	public:
	void SetItemData(const FMaterialAsset& NewItemData);

	FOnMaterialItemButtonClicked OnItemButtonClicked;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;

	virtual void NativeConstruct() override;
private:
	FMaterialAsset ItemData;

	UFUNCTION()
	void HandleItemButtonClick();
};
