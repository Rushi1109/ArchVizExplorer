// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/FloorPlacementMode.h"
#include "Actors/BuildingCreation/FloorActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Utilities/ArchVizUtility.h"

UFloorPlacementMode::UFloorPlacementMode() {}

void UFloorPlacementMode::Setup() {
	FloorActor = nullptr;
	bNewFloorStart = false;
	SubModeState = EBuildingSubModeState::Free;
}

void UFloorPlacementMode::Cleanup() {
	if (IsValid(FloorActor)) {
		if ((FloorActor->GetState() == EBuildingActorState::Previewing) || (FloorActor->GetState() == EBuildingActorState::Generating)) {
			FloorActor->DestroyActor();
		}
		else if (FloorActor->GetState() == EBuildingActorState::Moving) {
			FloorActor->SetState(EBuildingActorState::Selected);
		}
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

			InputMappingContext = NewObject<UInputMappingContext>(this);
			InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);
			InputMappingContext->MapKey(RKeyPressAction, EKeys::R);
			InputMappingContext->MapKey(MKeyPressAction, EKeys::M);

			EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &UFloorPlacementMode::HandleLeftClickAction);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UFloorPlacementMode::HandleRKeyPressAction);
			EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &UFloorPlacementMode::HandleMKeyPressAction);
		}
	}
}

void UFloorPlacementMode::EnterSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);

			Setup();
		}
	}
}

void UFloorPlacementMode::ExitSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);

			Cleanup();
		}
	}
}

void UFloorPlacementMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	if (HitResult.GetActor() && HitResult.GetActor()->IsA(AFloorActor::StaticClass())) {
		FloorActor = Cast<AFloorActor>(HitResult.GetActor());
		FloorActor->SetState(EBuildingActorState::Selected);

		// TODO:: Widget
		FloorActor->ShowPropertyPanel();
	}
	else {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FloorActor = GetWorld()->SpawnActor<AFloorActor>(FloorActorRef, SpawnParams);
		FloorActor->GenerateFloor(FVector{ 100, 100, 2 }, FVector{ 50, 50, 1 });
		FloorActor->SetState(EBuildingActorState::Previewing);
		SubModeState = EBuildingSubModeState::NewEntity;

		// TODO:: Material
	}
}

void UFloorPlacementMode::HandleOldEntityState() {
	SubModeState = EBuildingSubModeState::Free;
	FloorActor->SetState(EBuildingActorState::Selected);
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
			FloorActor->SetState(EBuildingActorState::Selected);
			SubModeState = EBuildingSubModeState::Free;
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
