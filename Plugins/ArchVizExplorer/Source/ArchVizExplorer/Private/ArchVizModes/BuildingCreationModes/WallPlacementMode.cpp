// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/WallPlacementMode.h"
#include "Actors/BuildingCreation/WallActor.h"
#include "ArchVizController.h"
#include "Utilities/ArchVizUtility.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "Widgets/PropertyPanelWidget.h"
#include "Actors/BuildingCreation/DoorActor.h"
#include "Actors/BuildingCreation/FloorActor.h"
#include "Actors/BuildingCreation/RoofActor.h"

UWallPlacementMode::UWallPlacementMode() {}

void UWallPlacementMode::Setup() {
	bNewWallStart = false;
	WallActor = nullptr;
	SubModeState = EBuildingSubModeState::Free;
}

void UWallPlacementMode::Cleanup() {
	if (IsValid(WallActor)) {
		if ((WallActor->GetState() == EBuildingActorState::Previewing) || (WallActor->GetState() == EBuildingActorState::Generating)) {
			WallActor->SetState(EBuildingActorState::None);
			WallActor->DestroyDoorComponents();
			WallActor->DestroyActor();
		}
		else {
			WallActor->SetState(EBuildingActorState::None);
		}
		WallActor = nullptr;
	}
}

void UWallPlacementMode::SetupInputMapping() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
			UInputAction* LeftClickAction = NewObject<UInputAction>(this);
			LeftClickAction->ValueType = EInputActionValueType::Boolean;

			UInputAction* RKeyPressAction = NewObject<UInputAction>(this);
			RKeyPressAction->ValueType = EInputActionValueType::Boolean;

			UInputAction* MKeyPressAction = NewObject<UInputAction>(this);
			MKeyPressAction->ValueType = EInputActionValueType::Boolean;

			UInputAction* DeleteKeyPressAction = NewObject<UInputAction>(this);
			DeleteKeyPressAction->ValueType = EInputActionValueType::Boolean;

			InputMappingContext = NewObject<UInputMappingContext>(this);
			InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);
			InputMappingContext->MapKey(RKeyPressAction, EKeys::R);
			InputMappingContext->MapKey(MKeyPressAction, EKeys::M);
			InputMappingContext->MapKey(DeleteKeyPressAction, EKeys::Delete);

			EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleLeftClickAction);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleRKeyPressAction);
			EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleMKeyPressAction);
			EIC->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleDeleteKeyPressAction);
		}
	}
}

void UWallPlacementMode::SetCurrentWallActor(AWallActor* Actor) {
	if (IsValid(WallActor)) {
		WallActor->SetState(EBuildingActorState::None);
		WallActor = nullptr;
	}

	WallActor = Actor;

	if (IsValid(WallActor)) {
		WallActor->SetState(EBuildingActorState::Selected);
	}
}

void UWallPlacementMode::EnterSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void UWallPlacementMode::ExitSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
		}
	}
}

void UWallPlacementMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	if (IsValid(WallActor)) {
		WallActor->SetState(EBuildingActorState::None);
		WallActor = nullptr;
	}

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		WallActor = Cast<AWallActor>(HitResult.GetActor());
		WallActor->SetState(EBuildingActorState::Selected);
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ADoorActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingModeEntity::DoorPlacement, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AFloorActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingModeEntity::FloorPlacement, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ARoofActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingModeEntity::RoofPlacement, HitResult.GetActor());
	}
	else {
		if (IsValid(WallActorRef)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			WallActor = GetWorld()->SpawnActor<AWallActor>(WallActorRef, SpawnParams);

			BindWidgetDelegates();

			WallActor->GenerateSegments();
			WallActor->SetState(EBuildingActorState::Previewing);
			SubModeState = EBuildingSubModeState::NewEntity;
			
			// TODO:: Preview Material
		}
	}
}

void UWallPlacementMode::HandleOldEntityState() {
	if (IsValid(WallActor)) {
		SubModeState = EBuildingSubModeState::Free;
		WallActor->SetState(EBuildingActorState::Selected);
	}
}

void UWallPlacementMode::HandleNewEntityState() {
	if (IsValid(WallActor)) {
		FHitResult HitResult = GetHitResult(TArray<AActor*>{WallActor});
		HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

		if (!bNewWallStart) {
			bNewWallStart = true;

			WallActor->SetStartLocation(HitResult.Location);
			WallActor->SetActorLocation(HitResult.Location);
			WallActor->SetState(EBuildingActorState::Generating);
		}
		else {
			bNewWallStart = false;

			WallActor->SetEndLocation(HitResult.Location);
			WallActor->UpdateWallLenghSlider();

			WallActor->SetState(EBuildingActorState::Selected);
			SubModeState = EBuildingSubModeState::Free;

			PlayerController->SetSuccess(FText::FromString("Completed The Wall Segment."));
		}
	}
}

void UWallPlacementMode::HandleLeftClickAction() {
	if (IsValid(WallActorRef)) {
		switch (SubModeState) {
		case EBuildingSubModeState::Free:
			HandleFreeState();
			break;
		case EBuildingSubModeState::OldEntity:
			HandleOldEntityState();
			break;
		case EBuildingSubModeState::NewEntity:
			HandleNewEntityState();
			break;
		}
	}
}

void UWallPlacementMode::HandleRKeyPressAction() {
	if (IsValid(WallActor)) {
		WallActor->RotateActor(90.0);
		WallActor->AdjustEdgeOffset();
	}
}

void UWallPlacementMode::HandleMKeyPressAction() {
	if (IsValid(WallActor)) {
		WallActor->SetState(EBuildingActorState::Moving);
		SubModeState = EBuildingSubModeState::OldEntity;
	}
}

void UWallPlacementMode::HandleDeleteKeyPressAction() {
	if (IsValid(WallActor)) {
		WallActor->SetState(EBuildingActorState::None);
		WallActor->DestroyDoorComponents();
		WallActor->DestroyActor();
		WallActor = nullptr;

		PlayerController->SetSuccess(FText::FromString("Deleted The Wall Segment."));
	}
}

void UWallPlacementMode::BindWidgetDelegates() {
	if (IsValid(WallActor) && IsValid(WallActor->PropertyPanel)) {
		WallActor->PropertyPanel->NewWallButton->OnClicked.AddDynamic(this, &UWallPlacementMode::HandleNewButtonClick);
		WallActor->PropertyPanel->DeleteWallButton->OnClicked.AddDynamic(this, &UWallPlacementMode::HandleDeleteButtonClick);
		WallActor->PropertyPanel->ClosePanelWallButton->OnClicked.AddDynamic(this, &UWallPlacementMode::HandleClosePanelButtonClick);
		WallActor->PropertyPanel->WallLengthSpinBox->OnValueChanged.AddDynamic(this, &UWallPlacementMode::HandleLengthSliderValueChange);
	}
}

void UWallPlacementMode::HandleNewButtonClick() {
	if (IsValid(WallActor)) {
		WallActor->SetState(EBuildingActorState::None);
		WallActor = nullptr;

		if(IsValid(WallActorRef)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			WallActor = GetWorld()->SpawnActor<AWallActor>(WallActorRef, SpawnParams);

			BindWidgetDelegates();

			WallActor->GenerateSegments();
			WallActor->SetState(EBuildingActorState::Previewing);
			SubModeState = EBuildingSubModeState::NewEntity;
		}
	}
}

void UWallPlacementMode::HandleDeleteButtonClick() {
	HandleDeleteKeyPressAction();
}

void UWallPlacementMode::HandleClosePanelButtonClick() {
	if (IsValid(WallActor)) {
		WallActor->SetState(EBuildingActorState::None);
		WallActor = nullptr;
	}
}

void UWallPlacementMode::HandleLengthSliderValueChange(float InValue) {
	if (IsValid(WallActor)) {
		WallActor->GenerateSegments(InValue);
	}
}