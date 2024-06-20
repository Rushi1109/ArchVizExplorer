// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/RoadConstructionMode.h"
#include "RoadActor.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

URoadConstructionMode::URoadConstructionMode() : PlayerController{ nullptr }, InputMappingContext{ nullptr }, RoadActor{ nullptr } {}

void URoadConstructionMode::SetupInputMapping() {
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EnhancedInputComponent);

	UInputAction* LeftClickAction = NewObject<UInputAction>(this);
	LeftClickAction->ValueType = EInputActionValueType::Boolean;

	InputMappingContext = NewObject<UInputMappingContext>(this);
	InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);

	EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleLeftClickAction);
}

void URoadConstructionMode::EnterMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void URoadConstructionMode::ExitMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
		}
	}
}

void URoadConstructionMode::InitParam(APlayerController* Controller) {
	PlayerController = Controller;
}

void URoadConstructionMode::HandleLeftClickAction() {
	FHitResult MouseHitResult{};

	FVector WorldLocation{}, WorldDirection{};

	if (PlayerController && PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceComplex = true;

		GetWorld()->LineTraceSingleByChannel(MouseHitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
	}

	if (IsValid(RoadActorRef) && !IsValid(RoadActor)) {
		RoadActor = NewObject<ARoadActor>(this, RoadActorRef);
	}

	if(IsValid(RoadActor)) {
		RoadActor->AddNewPoint(MouseHitResult.Location);
	}
}