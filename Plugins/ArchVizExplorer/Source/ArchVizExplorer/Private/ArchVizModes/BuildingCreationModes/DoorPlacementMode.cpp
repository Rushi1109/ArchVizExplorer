// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/DoorPlacementMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Actors/BuildingCreation/WallActor.h"

void UDoorPlacementMode::Setup() {
	DoorActor = nullptr;
	SubModeState = EBuildingSubModeState::Free;
}

void UDoorPlacementMode::Cleanup() {
	if (IsValid(DoorActor)) {
		if (DoorActor->GetState() == EBuildingActorState::Previewing) {
			DoorActor->DestroyActor();
		}
		else if (DoorActor->GetState() == EBuildingActorState::Moving) {
			DoorActor->SetState(EBuildingActorState::Selected);
		}
	}
}

void UDoorPlacementMode::EnterSubMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);

			Setup();
		}
	}
}

void UDoorPlacementMode::ExitSubMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);

			Cleanup();
		}
	}
}

void UDoorPlacementMode::SetupInputMapping() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
			InputMappingContext = NewObject<UInputMappingContext>();

			UInputAction* LeftClickAction = NewObject<UInputAction>();
			LeftClickAction->ValueType = EInputActionValueType::Boolean;

			UInputAction* RKeyPressAction = NewObject<UInputAction>(this);
			RKeyPressAction->ValueType = EInputActionValueType::Boolean;

			UInputAction* MKeyPressAction = NewObject<UInputAction>(this);
			MKeyPressAction->ValueType = EInputActionValueType::Boolean;

			UInputAction* OKeyPressAction = NewObject<UInputAction>(this);
			OKeyPressAction->ValueType = EInputActionValueType::Boolean;

			InputMappingContext = NewObject<UInputMappingContext>(this);
			InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);
			InputMappingContext->MapKey(RKeyPressAction, EKeys::R);
			InputMappingContext->MapKey(MKeyPressAction, EKeys::M);
			InputMappingContext->MapKey(OKeyPressAction, EKeys::O);

			EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &UDoorPlacementMode::HandleLeftClickAction);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UDoorPlacementMode::HandleRKeyPressAction);
			EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &UDoorPlacementMode::HandleMKeyPressAction);
			EIC->BindAction(OKeyPressAction, ETriggerEvent::Completed, this, &UDoorPlacementMode::HandleOKeyPressAction);
		}
	}
}
void UDoorPlacementMode::HandleLeftClickAction() {
	if (IsValid(DoorActorRef)) {
		FHitResult HitResult = GetHitResult();
		//HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

		switch (SubModeState) {
		case EBuildingSubModeState::Free:
			if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ADoorActor::StaticClass())) {
				DoorActor = Cast<ADoorActor>(HitResult.GetActor());
				DoorActor->SetState(EBuildingActorState::Selected);
				//TODO:: Widget
			}
			else {
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				DoorActor = GetWorld()->SpawnActor<ADoorActor>(DoorActorRef, SpawnParams);
				DoorActor->SetState(EBuildingActorState::Previewing);
				SubModeState = EBuildingSubModeState::NewEntity;
				//TODO:: Material
			}
			break;
		case EBuildingSubModeState::OldEntity:
			SubModeState = EBuildingSubModeState::Free;
			DoorActor->SetState(EBuildingActorState::Selected);
			break;
		case EBuildingSubModeState::NewEntity:
			if (IsValid(DoorActor)) {

				HitResult = GetHitResult(TArray<AActor*> {DoorActor});
				//HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

				if (HitResult.GetActor() && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
					AWallActor* WallActor = Cast<AWallActor>(HitResult.GetActor());

					if (IsValid(WallActor) && IsValid(HitResult.GetComponent())) {
						DoorActor->SetState(EBuildingActorState::Selected);
						WallActor->AttachDoorComponent(HitResult.GetComponent(), DoorActor);
						SubModeState = EBuildingSubModeState::Free;
					}
				}
			}
			break;
		}
	}
}

void UDoorPlacementMode::HandleRKeyPressAction() {
	if (IsValid(DoorActor)) {
		DoorActor->RotateActor(90.0);
	}
}

void UDoorPlacementMode::HandleMKeyPressAction() {
	if (IsValid(DoorActor)) {
		DoorActor->SetState(EBuildingActorState::Moving);
		SubModeState = EBuildingSubModeState::OldEntity;
	}
}

void UDoorPlacementMode::HandleOKeyPressAction() {
	if (IsValid(DoorActor)) {
		if (DoorActor->DoorComponent->GetRelativeRotation().Yaw == 0.0) {
			DoorActor->DoorComponent->SetRelativeRotation(FRotator{ 0.0, 90.0, 0.0 });
		}
		else {
			DoorActor->DoorComponent->SetRelativeRotation(FRotator{ 0.0, 0.0, 0.0 });
		}
	}
}
