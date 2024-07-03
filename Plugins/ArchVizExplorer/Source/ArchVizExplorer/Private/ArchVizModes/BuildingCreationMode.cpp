// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationMode.h"
#include "ArchVizController.h"
#include "Actors/BuildingCreation/WallActor.h"
#include "Actors/BuildingCreation/DoorActor.h"
#include "Actors/BuildingCreation/RoofActor.h"
#include "Actors/BuildingCreation/FloorActor.h"

UBuildingCreationMode::UBuildingCreationMode() : CurrentBuildingCreationSubMode{ nullptr }, BuildingModeEntity{ EBuildingModeEntity::WallPlacement }, WallPlacementMode{ nullptr }, DoorPlacementMode{ nullptr }, FloorPlacementMode{ nullptr }, RoofPlacementMode{ nullptr } {}

void UBuildingCreationMode::Setup() {
	if (IsValid(WidgetRef) && !IsValid(Widget)) {
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetRef, TEXT("Building Creation Widget"));
		if (auto* BuildingCreationWidget = Cast<UBuildingCreationWidget>(Widget)) {
			BuildingCreationWidget->OnBuildingModeEntityChange.AddUObject(this, &UBuildingCreationMode::HandleBuildingSubModeChange);
		}
	}

	if (IsValid(WallPlacementModeRef)) {
		WallPlacementMode = NewObject<UWallPlacementMode>(this, WallPlacementModeRef);
		WallPlacementMode->InitParams(PlayerController);
		WallPlacementMode->Setup();
		WallPlacementMode->OnOtherBuildingActorSelected.BindUObject(this, &UBuildingCreationMode::HandleBuildingActorSelected);
	}
	if (IsValid(DoorPlacementModeRef)) {
		DoorPlacementMode = NewObject<UDoorPlacementMode>(this, DoorPlacementModeRef);
		DoorPlacementMode->InitParams(PlayerController);
		DoorPlacementMode->Setup();
		DoorPlacementMode->OnOtherBuildingActorSelected.BindUObject(this, &UBuildingCreationMode::HandleBuildingActorSelected);
	}
	if (IsValid(FloorPlacementModeRef)) {
		FloorPlacementMode = NewObject<UFloorPlacementMode>(this, FloorPlacementModeRef);
		FloorPlacementMode->InitParams(PlayerController);
		FloorPlacementMode->Setup();
		FloorPlacementMode->OnOtherBuildingActorSelected.BindUObject(this, &UBuildingCreationMode::HandleBuildingActorSelected);
	}
	if (IsValid(RoofPlacementModeRef)) {
		RoofPlacementMode = NewObject<URoofPlacementMode>(this, RoofPlacementModeRef);
		RoofPlacementMode->InitParams(PlayerController);
		RoofPlacementMode->Setup();
		RoofPlacementMode->OnOtherBuildingActorSelected.BindUObject(this, &UBuildingCreationMode::HandleBuildingActorSelected);
	}

	if (IsValid(WallPlacementMode)) {
		BuildingModeEntity = EBuildingModeEntity::WallPlacement;
		CurrentBuildingCreationSubMode = WallPlacementMode;
	}
}

void UBuildingCreationMode::Cleanup() {
	if (IsValid(CurrentBuildingCreationSubMode)) {
		CurrentBuildingCreationSubMode->Cleanup();
	}
}

void UBuildingCreationMode::SetSubMode(UBuildingCreationSubMode* NewSubMode) {
	if (CurrentBuildingCreationSubMode) {
		CurrentBuildingCreationSubMode->Cleanup();
		CurrentBuildingCreationSubMode->ExitSubMode();
	}

	CurrentBuildingCreationSubMode = NewSubMode;

	if (CurrentBuildingCreationSubMode) {
		CurrentBuildingCreationSubMode->EnterSubMode();
		CurrentBuildingCreationSubMode->Setup();
	}
}

void UBuildingCreationMode::HandleBuildingSubModeChange(EBuildingModeEntity NewBuildingModeEntity) {
	BuildingModeEntity = NewBuildingModeEntity;

	UpdateBuildingModeEntity();

	if (UBuildingCreationWidget* BuildingWidget = Cast<UBuildingCreationWidget>(Widget)) {
		BuildingWidget->HighlightSelectedColour(NewBuildingModeEntity);
	}
}

void UBuildingCreationMode::HandleBuildingActorSelected(EBuildingModeEntity NewBuildingModeEntity, AActor* Actor) {
	HandleBuildingSubModeChange(NewBuildingModeEntity);
	UpdateSelectedActor(Actor);
}

void UBuildingCreationMode::UpdateBuildingModeEntity() {
	switch (BuildingModeEntity) {
	case EBuildingModeEntity::WallPlacement:
		SetSubMode(WallPlacementMode);
		break;
	case EBuildingModeEntity::DoorPlacement:
		SetSubMode(DoorPlacementMode);
		break;
	case EBuildingModeEntity::FloorPlacement:
		SetSubMode(FloorPlacementMode);
		break;
	case EBuildingModeEntity::RoofPlacement:
		SetSubMode(RoofPlacementMode);
		break;
	}
}

void UBuildingCreationMode::UpdateSelectedActor(AActor* Actor) {
	switch (BuildingModeEntity) {
	case EBuildingModeEntity::WallPlacement:
		if (auto* WallActor = Cast<AWallActor>(Actor)) {
			if (IsValid(WallPlacementMode)) {
				WallPlacementMode->SetCurrentWallActor(WallActor);
			}
		}
		break;
	case EBuildingModeEntity::DoorPlacement:
		if (auto* DoorActor = Cast<ADoorActor>(Actor)) {
			if (IsValid(DoorPlacementMode)) {
				DoorPlacementMode->SetCurrentDoorActor(DoorActor);
			}
		}
		break;
	case EBuildingModeEntity::FloorPlacement:
		if (auto* FloorActor = Cast<AFloorActor>(Actor)) {
			if (IsValid(FloorPlacementMode)) {
				FloorPlacementMode->SetCurrentFloorActor(FloorActor);
			}
		}
		break;
	case EBuildingModeEntity::RoofPlacement:
		if (auto* RoofActor = Cast<ARoofActor>(Actor)) {
			if (IsValid(RoofPlacementMode)) {
				RoofPlacementMode->SetCurrentRoofActor(RoofActor);
			}
		}
		break;
	}
}

void UBuildingCreationMode::SetupInputMapping() {
	if (IsValid(WallPlacementMode)) {
		WallPlacementMode->SetupInputMapping();
	}
	if (IsValid(DoorPlacementMode)) {
		DoorPlacementMode->SetupInputMapping();
	}
	if (IsValid(FloorPlacementMode)) {
		FloorPlacementMode->SetupInputMapping();
	}
	if (IsValid(RoofPlacementMode)) {
		RoofPlacementMode->SetupInputMapping();
	}
}

void UBuildingCreationMode::EnterMode() {
	ShowWidget();
	if (CurrentBuildingCreationSubMode) {
		CurrentBuildingCreationSubMode->EnterSubMode();
		CurrentBuildingCreationSubMode->Setup();

		PlayerController->SetSuccess(FText::FromString("Switched To Building Construction Mode."));
	}
}

void UBuildingCreationMode::ExitMode() {
	HideWidget();
	if (CurrentBuildingCreationSubMode) {
		CurrentBuildingCreationSubMode->Cleanup();
		CurrentBuildingCreationSubMode->ExitSubMode();
	}
}

void UBuildingCreationMode::BindPropertyPanelForSpawnedActor(ABuildingCreationActor* InBuildingActor) {
	if (IsValid(InBuildingActor)) {
		if (InBuildingActor->IsA(AWallActor::StaticClass())) {
			AWallActor* WallActor = Cast<AWallActor>(InBuildingActor);
			WallPlacementMode->SetWallActor(WallActor);
			WallPlacementMode->BindWidgetDelegates();
		}
		else if (InBuildingActor->IsA(ADoorActor::StaticClass())) {
			ADoorActor* DoorActor = Cast<ADoorActor>(InBuildingActor);
			DoorPlacementMode->SetDoorActor(DoorActor);
			DoorPlacementMode->BindWidgetDelegates();
		}
		else if (InBuildingActor->IsA(ARoofActor::StaticClass())) {
			ARoofActor* RoofActor = Cast<ARoofActor>(InBuildingActor);
			RoofPlacementMode->SetRoofActor(RoofActor);
			RoofPlacementMode->BindWidgetDelegates();
		}
		else if (InBuildingActor->IsA(AFloorActor::StaticClass())) {
			AFloorActor* FloorActor = Cast<AFloorActor>(InBuildingActor);
			FloorPlacementMode->SetFloorActor(FloorActor);
			FloorPlacementMode->BindWidgetDelegates();
		}
	}
}