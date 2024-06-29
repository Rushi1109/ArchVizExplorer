// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Button.h"
#include "DataAssets/InteriorDataAsset.h"
#include "InteriorScrollBox.h"
#include "InteriorDesignWidget.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UInteriorDesignWidget : public UUserWidget {
	GENERATED_BODY()

public:
		// Scroll Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInteriorScrollBox* InteriorScrollBox;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleChairButtonClick();

	UFUNCTION()
	void HandleTableButtonClick();

	UFUNCTION()
	void HandleBedButtonClick();

	UFUNCTION()
	void HandleCeilingLightButtonClick();

	UFUNCTION()
	void HandleLampButtonClick();

	UFUNCTION()
	void HandleFrameButtonClick();

	// Buttons
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

	// DataAssets
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* ChairDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* TableDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* BedDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* CeilingLightDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* LampDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* FrameDataAsset;
};
