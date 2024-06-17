// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizModeEnum.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/ModesMenuWidget.h"
#include "Widgets/RoadConstructionWidget.h"
#include "Widgets/BuildingCreationWidget.h"
#include "Widgets/InteriorDesignWidget.h"
#include "ArchVizController.generated.h"

class ARoadActor;
class UInputAction;
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

public:
	AArchVizController();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

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

private:
	UFUNCTION()
	void HandleModeChange(EArchVizMode ArchVizMode);

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizExplorer")
	EArchVizMode ArchVizMode;

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

	FInputModeGameAndUI InputMode;

	UPROPERTY(VisibleDefaultsOnly, Category = "Road")
	ARoadActor* RoadActor;

	UPROPERTY()
	UInputAction* RoadGeneratorLeftClickAction;

	UPROPERTY()
	UInputMappingContext* RoadGeneratorMappingContext;

	void UpdateMappingContext();
	void UpdateWidgets();
};
