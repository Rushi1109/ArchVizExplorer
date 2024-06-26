// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/WallPlacementMode.h"
#include "Actors/BuildingCreation/WallActor.h"
#include "Utilities/ArchVizUtility.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "Widgets/PropertyPanelWidget.h"

UWallPlacementMode::UWallPlacementMode() {}

void UWallPlacementMode::Setup() {
	bNewWallStart = false;
	WallActor = nullptr;
	SubModeState = EBuildingSubModeState::Free;
}

void UWallPlacementMode::Cleanup() {
	if (IsValid(WallActor)) {
		if ((WallActor->GetState() == EBuildingActorState::Previewing) || (WallActor->GetState() == EBuildingActorState::Generating)) {
			WallActor->DestroyActor();
		}
		WallActor->SetState(EBuildingActorState::None);
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

			InputMappingContext = NewObject<UInputMappingContext>(this);
			InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);
			InputMappingContext->MapKey(RKeyPressAction, EKeys::R);
			InputMappingContext->MapKey(MKeyPressAction, EKeys::M);

			EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleLeftClickAction);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleRKeyPressAction);
			EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleMKeyPressAction);
		}
	}
}

void UWallPlacementMode::EnterSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);

			Setup();
		}
	}
}

void UWallPlacementMode::ExitSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);

			Cleanup();
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

	if (HitResult.GetActor() && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		WallActor = Cast<AWallActor>(HitResult.GetActor());
		WallActor->SetState(EBuildingActorState::Selected);

		WallActor->ShowPropertyPanel();
	}
	else {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		WallActor = GetWorld()->SpawnActor<AWallActor>(WallActorRef, SpawnParams);
		BindWidgetButtons();

		WallActor->GenerateSegments();
		WallActor->SetState(EBuildingActorState::Previewing);
		SubModeState = EBuildingSubModeState::NewEntity;

		// Material
	}
}

void UWallPlacementMode::HandleOldEntityState() {
	SubModeState = EBuildingSubModeState::Free;
	WallActor->SetState(EBuildingActorState::Selected);
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
			UpdateWallLenghSlider();

			WallActor->SetState(EBuildingActorState::Selected);
			SubModeState = EBuildingSubModeState::Free;
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

void UWallPlacementMode::BindWidgetButtons() {
	if (IsValid(WallActor) && IsValid(WallActor->PropertyPanel) && WallActor->PropertyPanel->IsA(UPropertyPanelWidget::StaticClass())) {
		UPropertyPanelWidget* PropertyPanel = Cast<UPropertyPanelWidget>(WallActor->PropertyPanel);

		PropertyPanel->NewWallButton->OnClicked.AddDynamic(this, &UWallPlacementMode::HandleNewButtonClick);
		PropertyPanel->DeleteWallButton->OnClicked.AddDynamic(this, &UWallPlacementMode::HandleDeleteButtonClick);
		PropertyPanel->WallLengthSpinBox->OnValueChanged.AddDynamic(this, &UWallPlacementMode::HandleLengthSliderValueChange);
	}
}

void UWallPlacementMode::HandleNewButtonClick() {
	if (IsValid(WallActor)) {
		WallActor->SetState(EBuildingActorState::None);

		if(IsValid(WallActorRef)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			WallActor = GetWorld()->SpawnActor<AWallActor>(WallActorRef, SpawnParams);
			BindWidgetButtons();

			WallActor->GenerateSegments();
			WallActor->SetState(EBuildingActorState::Previewing);
			SubModeState = EBuildingSubModeState::NewEntity;
		}
	}
}

void UWallPlacementMode::HandleDeleteButtonClick() {
	if (IsValid(WallActor)) {
		WallActor->SetState(EBuildingActorState::None);
		WallActor->DestroyActor();
	}
}

void UWallPlacementMode::HandleLengthSliderValueChange(float InValue) {
	if (IsValid(WallActor)) {
		WallActor->GenerateSegments(InValue);
	}
}

void UWallPlacementMode::UpdateWallLenghSlider() {
	if (IsValid(WallActor) && IsValid(WallActor->PropertyPanel) && WallActor->PropertyPanel->IsA(UPropertyPanelWidget::StaticClass())) {
		UPropertyPanelWidget* PropertyPanel = Cast<UPropertyPanelWidget>(WallActor->PropertyPanel);

		double XDistance = FMath::Abs(WallActor->GetEndLocation().X - WallActor->GetStartLocation().X);
		double YDistance = FMath::Abs(WallActor->GetEndLocation().Y - WallActor->GetStartLocation().Y);

		if (XDistance >= YDistance) {
			PropertyPanel->WallLengthSpinBox->SetValue(XDistance);
		}
		else {
			PropertyPanel->WallLengthSpinBox->SetValue(YDistance);
		}
	}
}