// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/InteriorDesignMode.h"
#include "Actors/RoadActor.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

UInteriorDesignMode::UInteriorDesignMode() {}

void UInteriorDesignMode::Setup() {
	if (IsValid(WidgetRef) && !IsValid(Widget)) {
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetRef, "Interior Mode Widget");
	}
}

void UInteriorDesignMode::SetupInputMapping() {
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

		UInputAction* LeftClickAction = NewObject<UInputAction>(this);
		LeftClickAction->ValueType = EInputActionValueType::Boolean;

		InputMappingContext = NewObject<UInputMappingContext>(this);
		InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);

		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleLeftClickAction);
	}
}

void UInteriorDesignMode::EnterMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		ShowWidget();
	}
}

void UInteriorDesignMode::ExitMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
		}

		HideWidget();
	}
}

void UInteriorDesignMode::HandleLeftClickAction() {
	
}