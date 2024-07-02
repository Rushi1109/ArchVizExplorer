// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/ArchVizModeEnum.h"
#include "Enums/BuildingModeEntityEnum.h"
#include "ArchVizModes/ArchVizMode.h"
#include "ArchVizModes/RoadConstructionMode.h"
#include "ArchVizModes/BuildingCreationMode.h"
#include "ArchVizModes/InteriorDesignMode.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/ModesMenuWidget.h"
#include "Widgets/RoadConstructionWidget.h"
#include "Widgets/BuildingCreationWidget.h"
#include "Widgets/InteriorDesignWidget.h"
#include "ArchVizModes/SaveAndLoadMode.h"
#include "Widgets/NotificationWidget.h"
#include "ArchVizController.generated.h"

class ARoadActor;
class AWallActor;
class UInputMappingContext;

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API AArchVizController : public APlayerController {
	GENERATED_BODY()

public:
	friend class USaveAndLoadMode;

	AArchVizController();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void SetSuccess(FText SuccessText);
	void SetError(FText ErrorText);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UArchVizMode* CurrentMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArchVizMode | ModesMenu")
	TSubclassOf<UModesMenuWidget> ModesMenuWidgetRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArchVizMode | NotificationWidget")
	TSubclassOf<UNotificationWidget> NotificationWidgetRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RoadConstruction | Mode")
	TSubclassOf<URoadConstructionMode> RoadConstructionModeRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BuildingCreation | Mode")
	TSubclassOf<UBuildingCreationMode> BuildingCreationModeRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InteriorDesign | Mode")
	TSubclassOf<UInteriorDesignMode> InteriorDesignModeRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveAndLoad | Mode")
	TSubclassOf<USaveAndLoadMode> SaveAndLoadModeRef;

private:
	UFUNCTION()
	void HandleModeChange(EArchVizMode ArchVizMode);

	void SetArchVizMode(UArchVizMode* NewMode);

	UPROPERTY()
	UModesMenuWidget* ModesMenuWidget;

	UPROPERTY()
	UNotificationWidget* NotificationWidget;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizMode")
	EArchVizMode ArchVizMode;

	UPROPERTY()
	URoadConstructionMode* RoadConstructionMode;

	UPROPERTY()
	UBuildingCreationMode* BuildingCreationMode;

	UPROPERTY()
	UInteriorDesignMode* InteriorDesignMode;

	UPROPERTY()
	USaveAndLoadMode* SaveAndLoadMode;

	FInputModeGameAndUI InputMode;
};
