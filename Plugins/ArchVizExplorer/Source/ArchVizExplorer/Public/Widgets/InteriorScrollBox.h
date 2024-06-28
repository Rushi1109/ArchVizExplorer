// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "InteriorScrollableItem.h"
#include "InteriorScrollBox.generated.h"

DECLARE_DELEGATE_OneParam(FOnItemSelected, FInteriorAsset)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UInteriorScrollBox : public UUserWidget {
	GENERATED_BODY()

public:
	void PopulateWidget(UInteriorDataAsset* DataAsset);

	FOnItemSelected OnItemSelected;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UBorder* ScrollBoxBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UInteriorScrollableItem> InteriorScrollableItemRef;
};