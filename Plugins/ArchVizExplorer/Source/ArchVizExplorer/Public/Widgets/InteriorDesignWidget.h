// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Button.h"
#include "DataAssets/InteriorDataAsset.h"
#include "InteriorDesignWidget.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UInteriorDesignWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ChairButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* TableButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BedButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* CeilingLightButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* LampButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* FrameButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* ChairDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* TableDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* BedDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* CeilingLightDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* LampDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* FrameDataAsset;
};
