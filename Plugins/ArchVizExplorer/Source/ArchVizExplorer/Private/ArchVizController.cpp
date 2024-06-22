// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Actors/RoadActor.h"
#include "Actors/BuildingCreation/WallActor.h"

AArchVizController::AArchVizController() : CurrentMode{ nullptr }, ArchVizMode{ EArchVizMode::ViewMode } {}

void AArchVizController::BeginPlay() {
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetInputMode(InputMode);

	if (IsValid(ModesMenuWidgetRef)) {
		ModesMenuWidget = CreateWidget<UModesMenuWidget>(this, ModesMenuWidgetRef, TEXT("Modes Menu Widget"));
		ModesMenuWidget->AddToViewport(10);
		ModesMenuWidget->OnModeChanged.AddUObject(this, &AArchVizController::HandleModeChange);
	}

	if (IsValid(RoadConstructionModeRef)) {
		RoadConstructionMode = NewObject<URoadConstructionMode>(this, RoadConstructionModeRef);
		RoadConstructionMode->InitParam(this);
		RoadConstructionMode->Setup();
		RoadConstructionMode->SetupInputMapping();
	}
	if (IsValid(BuildingCreationModeRef)) {
		BuildingCreationMode = NewObject<UBuildingCreationMode>(this, BuildingCreationModeRef);
		BuildingCreationMode->InitParam(this);
		BuildingCreationMode->Setup();
		BuildingCreationMode->SetupInputMapping();
	}
	if (IsValid(InteriorDesignModeRef)) {
		InteriorDesignMode = NewObject<UInteriorDesignMode>(this, InteriorDesignModeRef);
		InteriorDesignMode->InitParam(this);
		InteriorDesignMode->Setup();
		InteriorDesignMode->SetupInputMapping();
	}
}

void AArchVizController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AArchVizController::SetupInputComponent() {
	Super::SetupInputComponent();

}

void AArchVizController::HandleModeChange(EArchVizMode NewArchVizMode) {
	ArchVizMode = NewArchVizMode;

	switch (ArchVizMode) {
	case EArchVizMode::ViewMode:
		break;
	case EArchVizMode::RoadConstruction:
		SetArchVizMode(RoadConstructionMode);
		break;
	case EArchVizMode::BuildingCreation:
		SetArchVizMode(BuildingCreationMode);
		break;
	case EArchVizMode::InteriorDesign:
		SetArchVizMode(InteriorDesignMode);
		break;
	}
}

void AArchVizController::SetArchVizMode(UArchVizMode* NewArchVizMode) {
	if (CurrentMode) {
		CurrentMode->ExitMode();
	}

	CurrentMode = NewArchVizMode;

	if (CurrentMode) {
		CurrentMode->EnterMode();
	}
}