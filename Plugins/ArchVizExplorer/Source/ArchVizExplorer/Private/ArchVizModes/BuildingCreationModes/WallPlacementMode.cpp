// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/WallPlacementMode.h"
#include "Actors/BuildingCreation/WallActor.h"
#include "Utilities/ArchVizUtility.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"

UWallPlacementMode::UWallPlacementMode() {}

void UWallPlacementMode::Setup() {
	bNewWallStart = false;
	WallActor = nullptr;
	WallSubModeState = EWallPlacementModeState::Free;
}

void UWallPlacementMode::SetupInputMapping() {
	if (IsValid(PlayerController)) {
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

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

		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleLeftClickAction);
		EnhancedInputComponent->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleRKeyPressAction);
		EnhancedInputComponent->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleMKeyPressAction);
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

void UWallPlacementMode::HandleLeftClickAction() {
	if (IsValid(WallActorRef)) {
		FHitResult HitResult;

		switch (WallSubModeState) {
		case EWallPlacementModeState::Free:
			HitResult = GetHitResult();
			HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

			if (HitResult.GetActor() && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
				WallActor = Cast<AWallActor>(HitResult.GetActor());
				WallActor->SetState(EBuildingActorState::Selected);

				WallActor->PropertyPanel->SetVisibility(ESlateVisibility::Visible);
			}
			else {
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				WallActor = GetWorld()->SpawnActor<AWallActor>(WallActorRef, SpawnParams);
				WallActor->GenerateSegments();
				WallActor->SetState(EBuildingActorState::Previewing);
				WallSubModeState = EWallPlacementModeState::NewWallSelected;

				// Material
			}
			break;
		case EWallPlacementModeState::OldWallSelected:
			WallSubModeState = EWallPlacementModeState::Free;
			WallActor->SetState(EBuildingActorState::Selected);
			break;
		case EWallPlacementModeState::NewWallSelected:
			if (IsValid(WallActor)) {
				HitResult = GetHitResult(TArray<AActor*>{WallActor});
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
					WallActor->SetState(EBuildingActorState::Selected);
					WallSubModeState = EWallPlacementModeState::Free;
				}
			}
			break;
		default:
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
		WallSubModeState = EWallPlacementModeState::OldWallSelected;
	}
}