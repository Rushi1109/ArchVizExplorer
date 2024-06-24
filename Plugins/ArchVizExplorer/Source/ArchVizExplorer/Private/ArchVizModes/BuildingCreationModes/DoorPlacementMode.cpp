// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/DoorPlacementMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

void UDoorPlacementMode::Setup() {
	
}

void UDoorPlacementMode::EnterSubMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void UDoorPlacementMode::ExitSubMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}
}

void UDoorPlacementMode::SetupInputMapping() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
			InputMappingContext = NewObject<UInputMappingContext>();

			//Left-Click
			UInputAction* LeftMouseClickAction = NewObject<UInputAction>();
			LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;


			InputMappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
			EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &UDoorPlacementMode::HandleLeftMouseClick);
		}
	}
}

void UDoorPlacementMode::HandleLeftMouseClick() {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, "Door Left Clicked");
}