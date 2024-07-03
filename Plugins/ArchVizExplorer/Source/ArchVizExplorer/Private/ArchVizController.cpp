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

	if (IsValid(NotificationWidgetRef)) {
		NotificationWidget = CreateWidget<UNotificationWidget>(this, NotificationWidgetRef);
		NotificationWidget->AddToViewport(1);
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
	if (IsValid(SaveAndLoadModeRef)) {
		SaveAndLoadMode = NewObject<USaveAndLoadMode>(this, SaveAndLoadModeRef);
		SaveAndLoadMode->InitParam(this);
		SaveAndLoadMode->Setup();
		SaveAndLoadMode->SetupInputMapping();
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
	case EArchVizMode::SaveAndLoad:
		SetArchVizMode(SaveAndLoadMode);
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

void AArchVizController::SetSuccess(FText SuccessText, float Time) {
	if (IsValid(NotificationWidget)) {
		NotificationWidget->SetSuccess(SuccessText, Time);
	}
}

void AArchVizController::SetError(FText ErrorText, float Time) {
	if (IsValid(NotificationWidget)) {
		NotificationWidget->SetError(ErrorText, Time);
	}
}

void AArchVizController::BindPropertyPanelForSpawnedActor(AArchVizActor* ArchVizActor) {
	if (IsValid(ArchVizActor)) {
		if (ArchVizActor->IsA(ARoadActor::StaticClass())) {
			ARoadActor* RoadActor = Cast<ARoadActor>(ArchVizActor);
			RoadConstructionMode->SetCurrentRoadActor(RoadActor);
			RoadConstructionMode->BindWidgetDelegates();
		}
		else if (ArchVizActor->IsA(AInteriorActor::StaticClass())) {
			AInteriorActor* InteriorActor = Cast<AInteriorActor>(ArchVizActor);
			InteriorDesignMode->SetCurrentInteriorActor(InteriorActor);
			InteriorDesignMode->BindWidgetDelegates();
		}
		else if (ArchVizActor->IsA(ABuildingCreationActor::StaticClass())) {
			ABuildingCreationActor* BuildingActor = Cast<ABuildingCreationActor>(ArchVizActor);

			BuildingCreationMode->BindPropertyPanelForSpawnedActor(BuildingActor);
		}
	}
}