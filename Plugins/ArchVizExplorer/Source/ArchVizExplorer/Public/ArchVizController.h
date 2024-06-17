// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/ModesMenuWidget.h"
#include "ArchVizController.generated.h"

class ARoadActor;
class UInputAction;
class UInputMappingContext;

enum class EArchVizMode : uint8 {
	ViewMode,
	RoadConstruction,
	BuildingCreation,
	InteriorDesign
};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Road")
	TSubclassOf<ARoadActor> RoadActorRef;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizExplorer")
	UModesMenuWidget* ModesMenuWidget;

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
};
