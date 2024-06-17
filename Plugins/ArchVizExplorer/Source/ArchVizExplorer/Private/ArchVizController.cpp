// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "RoadActor.h"

AArchVizController::AArchVizController() : RoadGeneratorLeftClickAction{ nullptr }, RoadGeneratorMappingContext{ nullptr } {

}

void AArchVizController::BeginPlay() {
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetInputMode(InputMode);
}

void AArchVizController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AArchVizController::SetupInputComponent() {
	Super::SetupInputComponent();

	SetupRoadGeneratorInput();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(RoadGeneratorMappingContext, 0);
	}
}

void AArchVizController::HandleRoadGeneratorLeftClick() {
	FHitResult MouseHitResult{};
	GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, MouseHitResult);

	if (!IsValid(RoadActor) && IsValid(RoadActorRef)) {
		RoadActor = NewObject<ARoadActor>(this, RoadActorRef);
	}

	RoadActor->AddNewPoint(MouseHitResult.Location);
}

void AArchVizController::SetupRoadGeneratorInput() {
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);

	RoadGeneratorLeftClickAction = NewObject<UInputAction>(this);
	RoadGeneratorLeftClickAction->ValueType = EInputActionValueType::Boolean;

	RoadGeneratorMappingContext = NewObject<UInputMappingContext>(this);
	RoadGeneratorMappingContext->MapKey(RoadGeneratorLeftClickAction, EKeys::LeftMouseButton);

	EnhancedInputComponent->BindAction(RoadGeneratorLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::HandleRoadGeneratorLeftClick);
}
