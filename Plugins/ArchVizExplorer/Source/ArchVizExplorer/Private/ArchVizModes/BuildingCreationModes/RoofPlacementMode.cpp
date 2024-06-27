// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/RoofPlacementMode.h"
#include "Actors/BuildingCreation/RoofActor.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Utilities/ArchVizUtility.h"
#include "Actors/BuildingCreation/WallActor.h"
#include "Actors/BuildingCreation/DoorActor.h"
#include "Actors/BuildingCreation/FloorActor.h"
#include "ProceduralMeshComponent.h"
#include "Widgets/PropertyPanelWidget.h"

URoofPlacementMode::URoofPlacementMode() {}

void URoofPlacementMode::Setup() {
	RoofActor = nullptr;
	bNewFloorStart = false;
	SubModeState = EBuildingSubModeState::Free;
}

void URoofPlacementMode::Cleanup() {
	if (IsValid(RoofActor)) {
		if ((RoofActor->GetState() == EBuildingActorState::Previewing) || (RoofActor->GetState() == EBuildingActorState::Generating)) {
			RoofActor->SetState(EBuildingActorState::None);
			RoofActor->DestroyActor();
		}
		else {
			RoofActor->SetState(EBuildingActorState::None);
		}
		RoofActor = nullptr;
	}
}

void URoofPlacementMode::EnterSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);

			Setup();
		}
	}
}

void URoofPlacementMode::ExitSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);

			Cleanup();
		}
	}
}

void URoofPlacementMode::SetupInputMapping() {
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

			EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &URoofPlacementMode::HandleLeftClickAction);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &URoofPlacementMode::HandleRKeyPressAction);
			EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &URoofPlacementMode::HandleMKeyPressAction);
			EIC->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &URoofPlacementMode::HandleDeleteKeyPressAction);
		}
	}
}

void URoofPlacementMode::SetCurrentRoofActor(ARoofActor* Actor) {
	if (IsValid(RoofActor)) {
		RoofActor->SetState(EBuildingActorState::None);
		RoofActor = nullptr;
	}

	RoofActor = Actor;

	if (IsValid(RoofActor)) {
		RoofActor->SetState(EBuildingActorState::Selected);
	}
}

void URoofPlacementMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	if (RoofActor) {
		RoofActor->SetState(EBuildingActorState::None);
		RoofActor = nullptr;
	}

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ARoofActor::StaticClass())) {
		RoofActor = Cast<ARoofActor>(HitResult.GetActor());
		RoofActor->SetState(EBuildingActorState::Selected);
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ADoorActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingModeEntity::DoorPlacement, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AFloorActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingModeEntity::FloorPlacement, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingModeEntity::WallPlacement, HitResult.GetActor());
	}
	else {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		RoofActor = GetWorld()->SpawnActor<ARoofActor>(RoofActorRef, SpawnParams);

		BindWidgetDelegates();

		RoofActor->GenerateRoof(FVector{ 100, 100, 10 }, FVector{ 50, 50, 5 });
		RoofActor->SetState(EBuildingActorState::Previewing);
		SubModeState = EBuildingSubModeState::NewEntity;

		// TODO:: Material
	}
}

void URoofPlacementMode::HandleOldEntityState() {
	SubModeState = EBuildingSubModeState::Free;
	RoofActor->SetState(EBuildingActorState::Selected);
}

void URoofPlacementMode::HandleNewEntityState() {
	if (IsValid(RoofActor)) {
		FHitResult HitResult = GetHitResult(TArray<AActor*>{RoofActor});
		HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

		if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
			if (!bNewFloorStart) {
				bNewFloorStart = true;

				RoofActor->SetStartLocation(HitResult.Location);
				RoofActor->SetActorLocation(HitResult.Location);
				RoofActor->SetState(EBuildingActorState::Generating);
			}
			else {
				bNewFloorStart = false;

				RoofActor->SetEndLocation(HitResult.Location);
				RoofActor->UpdateRoofDimensionSlider();
				RoofActor->SetState(EBuildingActorState::Selected);
				SubModeState = EBuildingSubModeState::Free;
			}
		}
	}
}

void URoofPlacementMode::HandleLeftClickAction() {
	if (IsValid(RoofActorRef)) {
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

void URoofPlacementMode::HandleRKeyPressAction() {
	if (IsValid(RoofActor)) {
		RoofActor->RotateActor(90.0);
	}
}

void URoofPlacementMode::HandleMKeyPressAction() {
	if (IsValid(RoofActor)) {
		RoofActor->SetState(EBuildingActorState::Moving);
		SubModeState = EBuildingSubModeState::OldEntity;
	}
}

void URoofPlacementMode::HandleDeleteKeyPressAction() {
	if (IsValid(RoofActor)) {
		RoofActor->SetState(EBuildingActorState::None);
		RoofActor->DestroyActor();
		RoofActor = nullptr;
	}
}

void URoofPlacementMode::BindWidgetDelegates() {
	if (IsValid(RoofActor) && IsValid(RoofActor->PropertyPanel)) {
		RoofActor->PropertyPanel->NewRoofButton->OnClicked.AddDynamic(this, &URoofPlacementMode::HandleNewButtonClick);
		RoofActor->PropertyPanel->DeleteRoofButton->OnClicked.AddDynamic(this, &URoofPlacementMode::HandleDeleteButtonClick);
		RoofActor->PropertyPanel->ClosePanelRoofButton->OnClicked.AddDynamic(this, &URoofPlacementMode::HandleClosePanelButtonClick);
		RoofActor->PropertyPanel->RoofLengthSpinBox->OnValueChanged.AddDynamic(this, &URoofPlacementMode::HandleDimensionSliderValueChange);
		RoofActor->PropertyPanel->RoofWidthSpinBox->OnValueChanged.AddDynamic(this, &URoofPlacementMode::HandleDimensionSliderValueChange);
		RoofActor->PropertyPanel->RoofHeightSpinBox->OnValueChanged.AddDynamic(this, &URoofPlacementMode::HandleDimensionSliderValueChange);
	}
}

void URoofPlacementMode::HandleNewButtonClick() {
	if (IsValid(RoofActor)) {
		RoofActor->SetState(EBuildingActorState::None);
		RoofActor = nullptr;

		if (IsValid(RoofActorRef)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			RoofActor = GetWorld()->SpawnActor<ARoofActor>(RoofActorRef, SpawnParams);

			BindWidgetDelegates();

			RoofActor->GenerateRoof(FVector{ 100, 100, 10 }, FVector{ 50, 50, 5 });
			RoofActor->SetState(EBuildingActorState::Previewing);
			SubModeState = EBuildingSubModeState::NewEntity;
		}
	}
}

void URoofPlacementMode::HandleDeleteButtonClick() {
	HandleDeleteKeyPressAction();
}

void URoofPlacementMode::HandleClosePanelButtonClick() {
	if (IsValid(RoofActor)) {
		RoofActor->SetState(EBuildingActorState::None);
		RoofActor = nullptr;
	}
}

void URoofPlacementMode::HandleDimensionSliderValueChange(float InValue) {
	if (IsValid(RoofActor) && IsValid(RoofActor->PropertyPanel)) {

		float RoofLength = RoofActor->PropertyPanel->RoofLengthSpinBox->GetValue();
		float RoofWidth = RoofActor->PropertyPanel->RoofWidthSpinBox->GetValue();
		float RoofHeight = RoofActor->PropertyPanel->RoofHeightSpinBox->GetValue();

		double EdgeOffset{ 10.0 };

		double XFloorLength = RoofActor->GetEndLocation().X - RoofActor->GetStartLocation().X;
		double YFloorLength = RoofActor->GetEndLocation().Y - RoofActor->GetStartLocation().Y;

		FVector RoofDimensions{ RoofLength + (2 * EdgeOffset), RoofWidth + (2 * EdgeOffset), RoofHeight };
		FVector Offset{ RoofLength / 2, RoofWidth / 2, RoofHeight / 2 };

		if (XFloorLength >= 0.0 && YFloorLength < 0.0) {
			Offset.Z *= -1.0;
		}
		else if (XFloorLength < 0.0 && YFloorLength >= 0.0) {
			Offset.Z *= -1.0;
		}

		RoofActor->GenerateRoof( RoofDimensions, Offset);
	}
}