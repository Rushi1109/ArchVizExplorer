// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/DoorPlacementMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Widgets/PropertyPanelWidget.h"
#include "Actors/BuildingCreation/WallActor.h"

void UDoorPlacementMode::Setup() {
	DoorActor = nullptr;
	SubModeState = EBuildingSubModeState::Free;
}

void UDoorPlacementMode::Cleanup() {
	if (IsValid(DoorActor)) {
		if ((DoorActor->GetState() == EBuildingActorState::Previewing) || (DoorActor->GetState() == EBuildingActorState::Moving)) {
			DoorActor->DestroyActor();
			DoorActor = nullptr;
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

void UDoorPlacementMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();

	if (IsValid(DoorActor)) {
		DoorActor->SetState(EBuildingActorState::None);
		DoorActor = nullptr;
	}

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ADoorActor::StaticClass())) {
		DoorActor = Cast<ADoorActor>(HitResult.GetActor());
		DoorActor->SetState(EBuildingActorState::Selected);
		//TODO:: Widget
	}
	else {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		DoorActor = GetWorld()->SpawnActor<ADoorActor>(DoorActorRef, SpawnParams);

		BindWidgetDelegates();

		DoorActor->SetState(EBuildingActorState::Previewing);
		SubModeState = EBuildingSubModeState::NewEntity;
		//TODO:: Material
	}
}

void UDoorPlacementMode::HandleOldEntityState() {}

void UDoorPlacementMode::HandleNewEntityState() {
	if (IsValid(DoorActor)) {
		FHitResult HitResult = GetHitResult(TArray<AActor*> {DoorActor});

		if (HitResult.GetActor() && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
			AWallActor* WallActor = Cast<AWallActor>(HitResult.GetActor());

			if (IsValid(WallActor) && IsValid(HitResult.GetComponent())) {
				DoorActor->SetState(EBuildingActorState::Selected);
				WallActor->AttachDoorComponent(HitResult.GetComponent(), DoorActor);
				SubModeState = EBuildingSubModeState::Free;
			}
		}
	}
}

void UDoorPlacementMode::HandleLeftClickAction() {
	if (IsValid(DoorActorRef)) {
		switch (SubModeState) {
		case EBuildingSubModeState::Free:
			HandleFreeState();
			break;
		case EBuildingSubModeState::OldEntity:
			[[fallthrough]];
		case EBuildingSubModeState::NewEntity:
			HandleNewEntityState();
			break;
		}
	}
}

void UDoorPlacementMode::HandleRKeyPressAction() {
	if (IsValid(DoorActor)) {
		DoorActor->RotateActor(180.0);
	}
}

void UDoorPlacementMode::HandleMKeyPressAction() {
	if (IsValid(DoorActor)) {
		if (AWallActor* WallActor = Cast<AWallActor>(DoorActor->GetAttachParentActor())) {
			WallActor->DetachDoorComponent(DoorActor);
		}

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

void UDoorPlacementMode::BindWidgetDelegates() {
	if (IsValid(DoorActor) && IsValid(DoorActor->PropertyPanel)) {
		DoorActor->PropertyPanel->NewDoorButton->OnClicked.AddDynamic(this, &UDoorPlacementMode::HandleNewButtonClick);
		DoorActor->PropertyPanel->DeleteDoorButton->OnClicked.AddDynamic(this, &UDoorPlacementMode::HandleDeleteButtonClick);
		DoorActor->PropertyPanel->ClosePanelDoorButton->OnClicked.AddDynamic(this, &UDoorPlacementMode::HandleClosePanelButtonClick);
	}
}

void UDoorPlacementMode::HandleNewButtonClick() {
	if (IsValid(DoorActor)) {
		DoorActor->SetState(EBuildingActorState::None);
		DoorActor = nullptr;

		if (IsValid(DoorActorRef)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			DoorActor = GetWorld()->SpawnActor<ADoorActor>(DoorActorRef, SpawnParams);

			BindWidgetDelegates();

			DoorActor->SetState(EBuildingActorState::Previewing);
			SubModeState = EBuildingSubModeState::NewEntity;
		}
	}
}

void UDoorPlacementMode::HandleDeleteButtonClick() {
	if (IsValid(DoorActor)) {
		DoorActor->SetState(EBuildingActorState::None);

		if (AWallActor* WallActor = Cast<AWallActor>(DoorActor->GetAttachParentActor())) {
			WallActor->DetachDoorComponent(DoorActor);
		}

		DoorActor->DestroyActor();
		DoorActor = nullptr;
	}
}

void UDoorPlacementMode::HandleClosePanelButtonClick() {
	if (IsValid(DoorActor)) {
		DoorActor->SetState(EBuildingActorState::None);
		DoorActor = nullptr;
	}
}