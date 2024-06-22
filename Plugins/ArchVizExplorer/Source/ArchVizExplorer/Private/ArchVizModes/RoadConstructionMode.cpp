// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/RoadConstructionMode.h"
#include "Actors/RoadActor.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

URoadConstructionMode::URoadConstructionMode() : RoadActor{ nullptr } {}

void URoadConstructionMode::Setup() {
	if (IsValid(RoadActorRef) && !IsValid(RoadActor)) {
		RoadActor = GetWorld()->SpawnActor<ARoadActor>(RoadActorRef, FTransform{});
	}

	if (IsValid(WidgetRef) && !IsValid(Widget)) {
		Widget = CreateWidget<URoadConstructionWidget>(GetWorld(), WidgetRef, "Road Mode Widget");
	}
}

void URoadConstructionMode::SetupInputMapping() {
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

		UInputAction* LeftClickAction = NewObject<UInputAction>(this);
		LeftClickAction->ValueType = EInputActionValueType::Boolean;

		InputMappingContext = NewObject<UInputMappingContext>(this);
		InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);

		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleLeftClickAction);
	}
}

void URoadConstructionMode::EnterMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		ShowWidget();
	}
}

void URoadConstructionMode::ExitMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
		}

		HideWidget();
	}
}

void URoadConstructionMode::HandleLeftClickAction() {
	if (IsValid(RoadActor)) {
		FHitResult HitResult{};

		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(RoadActor);

		HitResult = RoadActor->GetHitResult(IgnoredActors);
		RoadActor->AddNewPoint(HitResult.Location);
	}
}