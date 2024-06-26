// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/RoofPlacementMode.h"
#include "Actors/BuildingCreation/RoofActor.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Utilities/ArchVizUtility.h"
#include "Actors/BuildingCreation/WallActor.h"

URoofPlacementMode::URoofPlacementMode() {}

void URoofPlacementMode::Setup() {
	RoofActor = nullptr;
	bNewFloorStart = false;
	SubModeState = EBuildingSubModeState::Free;
}

void URoofPlacementMode::Cleanup() {
	if (IsValid(RoofActor)) {
		if ((RoofActor->GetState() == EBuildingActorState::Previewing) || (RoofActor->GetState() == EBuildingActorState::Generating)) {
			RoofActor->DestroyActor();
		}
		else if (RoofActor->GetState() == EBuildingActorState::Moving) {
			RoofActor->SetState(EBuildingActorState::Selected);
		}
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

			InputMappingContext = NewObject<UInputMappingContext>(this);
			InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);
			InputMappingContext->MapKey(RKeyPressAction, EKeys::R);
			InputMappingContext->MapKey(MKeyPressAction, EKeys::M);

			EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &URoofPlacementMode::HandleLeftClickAction);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &URoofPlacementMode::HandleRKeyPressAction);
			EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &URoofPlacementMode::HandleMKeyPressAction);
		}
	}
}

void URoofPlacementMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	if (HitResult.GetActor() && HitResult.GetActor()->IsA(ARoofActor::StaticClass())) {
		RoofActor = Cast<ARoofActor>(HitResult.GetActor());
		RoofActor->SetState(EBuildingActorState::Selected);

		// TODO:: Widget
		RoofActor->ShowPropertyPanel();
	}
	else {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		RoofActor = GetWorld()->SpawnActor<ARoofActor>(RoofActorRef, SpawnParams);
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