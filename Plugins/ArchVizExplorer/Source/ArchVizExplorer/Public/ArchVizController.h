// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/ArchVizMode.h"
#include "Enums/BuildingModeEntity.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/ModesMenuWidget.h"
#include "Widgets/RoadConstructionWidget.h"
#include "Widgets/BuildingCreationWidget.h"
#include "Widgets/InteriorDesignWidget.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArchVizExplorer")
	TSubclassOf<UModesMenuWidget> ModesMenuWidgetRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArchVizExplorer")
	TSubclassOf<URoadConstructionWidget> RoadConstructionWidgetRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArchVizExplorer")
	TSubclassOf<UBuildingCreationWidget> BuildingCreationWidgetRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArchVizExplorer")
	TSubclassOf<UInteriorDesignWidget> InteriorDesignWidgetRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Road")
	TSubclassOf<ARoadActor> RoadActorRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wall")
	TSubclassOf<AWallActor> WallActorRef;

public:
	AArchVizController();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

private:
	UFUNCTION()
	void HandleModeChange(EArchVizMode ArchVizMode);

	UFUNCTION()
	void HandleBuildingModeEntityChange(EBuildingModeEntity NewBuildingModeEntity);

	FHitResult GetHitResult() const;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizExplorer")
	EArchVizMode ArchVizMode;

	UPROPERTY(VisibleDefaultsOnly, Category = "BuildingCreationMode", meta = (EditCondition = "ArchVizMode == EArchVizMode::BuildingCreation"))
	EBuildingModeEntity BuildingModeEntity;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizExplorer")
	UModesMenuWidget* ModesMenuWidget;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizExplorer")
	URoadConstructionWidget* RoadConstructionWidget;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizExplorer")
	UBuildingCreationWidget* BuildingCreationWidget;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizExplorer")
	UInteriorDesignWidget* InteriorDesignWidget;

	// Road Generator
	void SetupRoadGeneratorInput();

	UFUNCTION(BlueprintCallable)
	void HandleRoadGeneratorLeftClick();

	UPROPERTY(VisibleDefaultsOnly, Category = "Road")
	ARoadActor* RoadActor;

	UPROPERTY()
	UInputMappingContext* RoadGeneratorMappingContext;

	// Wall Generator
	void SetupWallGeneratorInput();

	UFUNCTION(BlueprintCallable)
	void HandleWallGeneratorLeftClick();

	UFUNCTION(BlueprintCallable)
	void HandleWallGeneratorRKeyPress();

	UPROPERTY(VisibleDefaultsOnly, Category = "Wall")
	AWallActor* WallActor;

	UPROPERTY()
	UInputMappingContext* WallGeneratorMappingContext;

	void CleanBeforeChange();
	void UpdateMappingContext();
	void UpdateWidgets();

	FVector SnapToGrid(const FVector& WorldLocation);

	FInputModeGameAndUI InputMode;
};
