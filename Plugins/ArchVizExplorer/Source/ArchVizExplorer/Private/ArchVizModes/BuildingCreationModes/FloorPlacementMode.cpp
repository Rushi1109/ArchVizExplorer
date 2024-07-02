// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/FloorPlacementMode.h"
#include "Actors/BuildingCreation/FloorActor.h"
#include "ArchVizController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Widgets/PropertyPanelWidget.h"
#include "Utilities/ArchVizUtility.h"
#include "Actors/BuildingCreation/DoorActor.h"
#include "Actors/BuildingCreation/RoofActor.h"
#include "Actors/BuildingCreation/WallActor.h"

UFloorPlacementMode::UFloorPlacementMode() {}

void UFloorPlacementMode::Setup() {
	FloorActor = nullptr;
	bNewFloorStart = false;
	SubModeState = EBuildingSubModeState::Free;
}

void UFloorPlacementMode::Cleanup() {
	if (IsValid(FloorActor)) {
		if ((FloorActor->GetState() == EBuildingActorState::Previewing) || (FloorActor->GetState() == EBuildingActorState::Generating)) {
			FloorActor->SetState(EBuildingActorState::None);
			FloorActor->DestroyActor();
		}
		else {
			FloorActor->SetState(EBuildingActorState::None);
		}
		FloorActor = nullptr;
	}
}

void UFloorPlacementMode::SetupInputMapping() {
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

			EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &UFloorPlacementMode::HandleLeftClickAction);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UFloorPlacementMode::HandleRKeyPressAction);
			EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &UFloorPlacementMode::HandleMKeyPressAction);
			EIC->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &UFloorPlacementMode::HandleDeleteKeyPressAction);
		}
	}
}

void UFloorPlacementMode::SetCurrentFloorActor(AFloorActor* Actor) {
	if (IsValid(FloorActor)) {
		FloorActor->SetState(EBuildingActorState::None);
		FloorActor = nullptr;
	}

	FloorActor = Actor;

	if (IsValid(FloorActor)) {
		FloorActor->SetState(EBuildingActorState::Selected);
	}

}

void UFloorPlacementMode::EnterSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void UFloorPlacementMode::ExitSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
		}
	}
}

void UFloorPlacementMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	if (IsValid(FloorActor)) {
		FloorActor->SetState(EBuildingActorState::None);
		FloorActor = nullptr;
	}

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AFloorActor::StaticClass())) {
		FloorActor = Cast<AFloorActor>(HitResult.GetActor());
		FloorActor->SetState(EBuildingActorState::Selected);
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ADoorActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingModeEntity::DoorPlacement, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ARoofActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingModeEntity::RoofPlacement, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingModeEntity::WallPlacement, HitResult.GetActor());
	}
	else {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FloorActor = GetWorld()->SpawnActor<AFloorActor>(FloorActorRef, SpawnParams);

		BindWidgetDelegates();

		FloorActor->GenerateFloor(FVector{ 100, 100, 2 }, FVector{ 50, 50, 1 });
		FloorActor->SetState(EBuildingActorState::Previewing);
		SubModeState = EBuildingSubModeState::NewEntity;

		// TODO:: Material
	}
}

void UFloorPlacementMode::HandleOldEntityState() {
	if (IsValid(FloorActor)) {
		SubModeState = EBuildingSubModeState::Free;
		FloorActor->SetState(EBuildingActorState::Selected);
	}
}

void UFloorPlacementMode::HandleNewEntityState() {
	if (IsValid(FloorActor)) {
		FHitResult HitResult = GetHitResult(TArray<AActor*>{FloorActor});
		HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

		if (!bNewFloorStart) {
			bNewFloorStart = true;

			FloorActor->SetStartLocation(HitResult.Location);
			FloorActor->SetActorLocation(HitResult.Location);
			FloorActor->SetState(EBuildingActorState::Generating);
		}
		else {
			bNewFloorStart = false;

			FloorActor->SetEndLocation(HitResult.Location);
			FloorActor->UpdateFloorDimensionSlider();
			FloorActor->SetState(EBuildingActorState::Selected);
			SubModeState = EBuildingSubModeState::Free;

			PlayerController->SetSuccess(FText::FromString("Completed The Floor Segment."));
		}
	}
}

void UFloorPlacementMode::HandleLeftClickAction() {
	if (IsValid(FloorActorRef)) {
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

void UFloorPlacementMode::HandleRKeyPressAction() {
	if (IsValid(FloorActor)) {
		FloorActor->RotateActor(90.0);
	}
}

void UFloorPlacementMode::HandleMKeyPressAction() {
	if (IsValid(FloorActor)) {
		FloorActor->SetState(EBuildingActorState::Moving);
		SubModeState = EBuildingSubModeState::OldEntity;
	}
}

void UFloorPlacementMode::HandleDeleteKeyPressAction() {
	if (IsValid(FloorActor)) {
		FloorActor->SetState(EBuildingActorState::None);
		FloorActor->DestroyActor();
		FloorActor = nullptr;

		PlayerController->SetSuccess(FText::FromString("Deleted The Floor Segment."));
	}
}

void UFloorPlacementMode::BindWidgetDelegates() {
	if (IsValid(FloorActor) && IsValid(FloorActor->PropertyPanel)) {
		FloorActor->PropertyPanel->NewFloorButton->OnClicked.AddDynamic(this, &UFloorPlacementMode::HandleNewButtonClick);
		FloorActor->PropertyPanel->DeleteFloorButton->OnClicked.AddDynamic(this, &UFloorPlacementMode::HandleDeleteButtonClick);
		FloorActor->PropertyPanel->ClosePanelFloorButton->OnClicked.AddDynamic(this, &UFloorPlacementMode::HandleClosePanelButtonClick);
		FloorActor->PropertyPanel->FloorLengthSpinBox->OnValueChanged.AddDynamic(this, &UFloorPlacementMode::HandleDimensionSliderValueChange);
		FloorActor->PropertyPanel->FloorWidthSpinBox->OnValueChanged.AddDynamic(this, &UFloorPlacementMode::HandleDimensionSliderValueChange);
		FloorActor->PropertyPanel->FloorHeightSpinBox->OnValueChanged.AddDynamic(this, &UFloorPlacementMode::HandleDimensionSliderValueChange);
	}
}

void UFloorPlacementMode::HandleNewButtonClick() {
	if (IsValid(FloorActor)) {
		FloorActor->SetState(EBuildingActorState::None);
		FloorActor = nullptr;

		if (IsValid(FloorActorRef)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FloorActor = GetWorld()->SpawnActor<AFloorActor>(FloorActorRef, SpawnParams);

			BindWidgetDelegates();

			FloorActor->GenerateFloor(FVector{ 100, 100, 2 }, FVector{ 50, 50, 1 });
			FloorActor->SetState(EBuildingActorState::Previewing);
			SubModeState = EBuildingSubModeState::NewEntity;
		}
	}
}

void UFloorPlacementMode::HandleDeleteButtonClick() {
	HandleDeleteKeyPressAction();
}

void UFloorPlacementMode::HandleClosePanelButtonClick() {
	if (IsValid(FloorActor)) {
		FloorActor->SetState(EBuildingActorState::None);
		FloorActor = nullptr;
	}
}

void UFloorPlacementMode::HandleDimensionSliderValueChange(float InValue) {
	if (IsValid(FloorActor) && IsValid(FloorActor->PropertyPanel)) {

		float FloorLength = FloorActor->PropertyPanel->FloorLengthSpinBox->GetValue();
		float FloorWidth = FloorActor->PropertyPanel->FloorWidthSpinBox->GetValue();
		float FloorHeight = FloorActor->PropertyPanel->FloorHeightSpinBox->GetValue();

		double EdgeOffset{ 10.0 };

		double XFloorLength = FloorActor->GetEndLocation().X - FloorActor->GetStartLocation().X;
		double YFloorLength = FloorActor->GetEndLocation().Y - FloorActor->GetStartLocation().Y;

		FVector RoofDimensions{ FloorLength + (2 * EdgeOffset), FloorWidth + (2 * EdgeOffset), FloorHeight };
		FVector Offset{ FloorLength / 2, FloorWidth / 2, FloorHeight / 2 };

		if (XFloorLength >= 0.0 && YFloorLength < 0.0) {
			Offset.Z *= -1.0;
		}
		else if (XFloorLength < 0.0 && YFloorLength >= 0.0) {
			Offset.Z *= -1.0;
		}

		FloorActor->GenerateFloor(RoofDimensions, Offset);
	}
}