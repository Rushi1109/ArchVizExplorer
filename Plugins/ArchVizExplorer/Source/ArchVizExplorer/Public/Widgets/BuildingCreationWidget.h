// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/BuildingModeEntity.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/Image.h"
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

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleWallEntityButtonClick(FGeometry Geometry, FPointerEvent& PointerEvent);

	UFUNCTION()
	void HandleDoorEntityButtonClick(FGeometry Geometry, FPointerEvent& PointerEvent);

	UFUNCTION()
	void HandleFloorEntityButtonClick(FGeometry Geometry, FPointerEvent& PointerEvent);

	UFUNCTION()
	void HandleRoofEntityButtonClick(FGeometry Geometry, FPointerEvent& PointerEvent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Canvas;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UImage* WallIMG;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UImage* DoorIMG;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UImage* FloorIMG;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UImage* RoofIMG;
};
