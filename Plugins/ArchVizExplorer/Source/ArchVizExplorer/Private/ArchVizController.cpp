// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Actors/RoadActor.h"
#include "Actors/BuildingCreation/WallActor.h"

AArchVizController::AArchVizController() : CurrentMode{ nullptr }, ArchVizMode{ EArchVizMode::ViewMode }, BuildingModeEntity{ EBuildingModeEntity::None } {

}

void AArchVizController::BeginPlay() {
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetInputMode(InputMode);

	if (IsValid(ModesMenuWidgetRef)) {
		ModesMenuWidget = CreateWidget<UModesMenuWidget>(this, ModesMenuWidgetRef, TEXT("Modes Menu Widget"));
		ModesMenuWidget->AddToViewport(10);
		ModesMenuWidget->OnModeChanged.AddUObject(this, &AArchVizController::HandleModeChange);

		if (IsValid(RoadConstructionWidgetRef)) {
			RoadConstructionWidget = CreateWidget<URoadConstructionWidget>(this, RoadConstructionWidgetRef, TEXT("Road Construction Widget"));
		}

		if (IsValid(BuildingCreationWidgetRef)) {
			BuildingCreationWidget = CreateWidget<UBuildingCreationWidget>(this, BuildingCreationWidgetRef, TEXT("Building Creation Widget"));
			BuildingCreationWidget->OnBuildingModeEntityChange.AddUObject(this, &AArchVizController::HandleBuildingModeEntityChange);
		}

		if (IsValid(InteriorDesignWidgetRef)) {
			InteriorDesignWidget = CreateWidget<UInteriorDesignWidget>(this, InteriorDesignWidgetRef, TEXT("Interior Design Widget"));
		}
	}

	if (IsValid(RoadConstructionModeRef)) {
		RoadConstructionMode = NewObject<URoadConstructionMode>(this, RoadConstructionModeRef);
		RoadConstructionMode->InitParam(this);
		RoadConstructionMode->SetupInputMapping();
	}
	if (IsValid(BuildingCreationModeRef)) {
		BuildingCreationMode = NewObject<UBuildingCreationMode>(this, BuildingCreationModeRef);
		BuildingCreationMode->InitParam(this);
		BuildingCreationMode->SetBuildingModeEntity(BuildingModeEntity);
		BuildingCreationMode->SetupSubModes();
		BuildingCreationMode->SetupInputMapping();
	}
}

void AArchVizController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (ArchVizMode == EArchVizMode::BuildingCreation) {
		if(IsValid(BuildingCreationMode)) {
			BuildingCreationMode->PreviewSegment();
		}
	}
}

void AArchVizController::SetupInputComponent() {
	Super::SetupInputComponent();

}

void AArchVizController::HandleModeChange(EArchVizMode NewArchVizMode) {
	CleanBeforeChange();
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
		break;
	default:
		break;
	}

	UpdateWidgets();
}

void AArchVizController::SetArchVizMode(IArchVizMode* NewArchVizMode) {
	if (CurrentMode) {
		CurrentMode->ExitMode();
	}

	CurrentMode = NewArchVizMode;

	if (CurrentMode) {
		CurrentMode->EnterMode();
	}
}

void AArchVizController::HandleBuildingModeEntityChange(EBuildingModeEntity NewBuildingModeEntity) {
	BuildingModeEntity = NewBuildingModeEntity;

	BuildingCreationMode->SetBuildingModeEntity(NewBuildingModeEntity);
}

void AArchVizController::CleanBeforeChange() {
	if(ArchVizMode == EArchVizMode::BuildingCreation) {
		BuildingModeEntity = EBuildingModeEntity::None;
		BuildingCreationMode->SetBuildingModeEntity(EBuildingModeEntity::None);
	}
}

void AArchVizController::UpdateWidgets() {
	switch (ArchVizMode) {
	case EArchVizMode::ViewMode:
		if (IsValid(BuildingCreationWidget) && BuildingCreationWidget->IsInViewport()) {
			BuildingCreationWidget->RemoveFromParent();
		}
		if (IsValid(InteriorDesignWidget) && InteriorDesignWidget->IsInViewport()) {
			InteriorDesignWidget->RemoveFromParent();
		}
		if (IsValid(RoadConstructionWidget) && RoadConstructionWidget->IsInViewport()) {
			RoadConstructionWidget->RemoveFromParent();
		}
		break;
	case EArchVizMode::RoadConstruction:
		if (IsValid(BuildingCreationWidget) && BuildingCreationWidget->IsInViewport()) {
			BuildingCreationWidget->RemoveFromParent();
		}
		if (IsValid(InteriorDesignWidget) && InteriorDesignWidget->IsInViewport()) {
			InteriorDesignWidget->RemoveFromParent();
		}
		if (!RoadConstructionWidget->IsInViewport()) {
			RoadConstructionWidget->AddToViewport();
		}
		break;
	case EArchVizMode::BuildingCreation:
		if (!BuildingCreationWidget->IsInViewport()) {
			BuildingCreationWidget->AddToViewport();
		}
		if (IsValid(InteriorDesignWidget) && InteriorDesignWidget->IsInViewport()) {
			InteriorDesignWidget->RemoveFromParent();
		}
		if (IsValid(RoadConstructionWidget) && RoadConstructionWidget->IsInViewport()) {
			RoadConstructionWidget->RemoveFromParent();
		}
		break;
	case EArchVizMode::InteriorDesign:
		if (IsValid(BuildingCreationWidget) && BuildingCreationWidget->IsInViewport()) {
			BuildingCreationWidget->RemoveFromParent();
		}
		if (!InteriorDesignWidget->IsInViewport()) {
			InteriorDesignWidget->AddToViewport();
		}
		if (IsValid(RoadConstructionWidget) && RoadConstructionWidget->IsInViewport()) {
			RoadConstructionWidget->RemoveFromParent();
		}
		break;
	}
}