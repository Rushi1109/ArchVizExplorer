// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "Widgets/MaterialItem.h"
#include "MaterialScrollBox.generated.h"

DECLARE_DELEGATE_OneParam(FOnMaterialItemSelected, FMaterialAsset)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UMaterialScrollBox : public UUserWidget {
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void PopulateWidget(UMaterialDataAsset* DataAsset);

	FOnMaterialItemSelected OnItemSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMaterialItem> MaterialItemWidgetRef;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBox;
};
