// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/RoofPlacementMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

void URoofPlacementMode::Setup() {

}

void URoofPlacementMode::EnterSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void URoofPlacementMode::ExitSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}
}

void URoofPlacementMode::SetupInputMapping() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
			InputMappingContext = NewObject<UInputMappingContext>();

			//Left-Click
			auto* LeftMouseClickAction = NewObject<UInputAction>();
			LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;


			InputMappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
			EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &URoofPlacementMode::HandleLeftMouseClick);
		}
	}
}

void URoofPlacementMode::HandleLeftMouseClick() {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, "Door Left Clicked");
}