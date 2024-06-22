// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/FloorPlacementMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UFloorPlacementMode::UFloorPlacementMode() : FloorActor{ nullptr }, bIsDragging{ false } {}

void UFloorPlacementMode::Setup() {
	
}

void UFloorPlacementMode::SetupInputMapping() {
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EnhancedInputComponent);

	UInputAction* LeftClickAction = NewObject<UInputAction>(this);
	LeftClickAction->ValueType = EInputActionValueType::Boolean;

	InputMappingContext = NewObject<UInputMappingContext>(this);
	InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);

	EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &UFloorPlacementMode::HandleLeftClickAction);
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

void UFloorPlacementMode::HandleLeftClickAction() {

}