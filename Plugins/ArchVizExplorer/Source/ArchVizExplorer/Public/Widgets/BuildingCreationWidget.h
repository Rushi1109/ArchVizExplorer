// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/BuildingModeEntityEnum.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/Button.h"
#include "BuildingCreationWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBuildingModeEntityChange, EBuildingModeEntity);

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UBuildingCreationWidget : public UUserWidget {
	GENERATED_BODY()
public:
	FOnBuildingModeEntityChange OnBuildingModeEntityChange;

	void HighlightSelectedColour(EBuildingModeEntity NewBuildingModeEntity);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleWallEntityButtonClick();

	UFUNCTION()
	void HandleDoorEntityButtonClick();

	UFUNCTION()
	void HandleFloorEntityButtonClick();

	UFUNCTION()
	void HandleRoofEntityButtonClick();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Canvas;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* WallButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* DoorButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* FloorButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* RoofButton;

private:
	void SetNormalTint(FButtonStyle& ButtonStyle);
	void SetHighlightedTint(FButtonStyle& ButtonStyle);
};
