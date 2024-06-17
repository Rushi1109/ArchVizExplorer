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

	if (IsValid(ModesMenuWidgetRef)) {
		ModesMenuWidget = CreateWidget<UModesMenuWidget>(this, ModesMenuWidgetRef, TEXT("Modes Menu Widget"));
		ModesMenuWidget->AddToViewport(10);
		ModesMenuWidget->OnModeChanged.AddUObject(this, &AArchVizController::HandleModeChange);

		if (IsValid(RoadConstructionWidgetRef)) {
			RoadConstructionWidget = CreateWidget<URoadConstructionWidget>(this, RoadConstructionWidgetRef, TEXT("Road Construction Widget"));
			RoadConstructionWidget->AddToViewport(0);
		}

		if (IsValid(RoadConstructionWidgetRef)) {
			BuildingCreationWidget = CreateWidget<UBuildingCreationWidget>(this, BuildingCreationWidgetRef, TEXT("Building Creation Widget"));
			BuildingCreationWidget->AddToViewport(0);
		}

		if (IsValid(RoadConstructionWidgetRef)) {
			InteriorDesignWidget = CreateWidget<UInteriorDesignWidget>(this, InteriorDesignWidgetRef, TEXT("Interior Design Widget"));
			InteriorDesignWidget->AddToViewport(0);
		}
	}
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

void AArchVizController::HandleModeChange(EArchVizMode NewArchVizMode) {
	ArchVizMode = NewArchVizMode;

	UpdateMappingContext();
	UpdateWidgets();
}

void AArchVizController::HandleRoadGeneratorLeftClick() {
	FHitResult MouseHitResult{};
	GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, MouseHitResult);

	if (IsValid(RoadActorRef) && !IsValid(RoadActor)) {
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

void AArchVizController::UpdateMappingContext() {
	// TODO
}

void AArchVizController::UpdateWidgets() {
	switch (ArchVizMode) {
	case EArchVizMode::ViewMode:
		if (IsValid(BuildingCreationWidget) && BuildingCreationWidget->IsInViewport()) {
			BuildingCreationWidget->RemoveFromParent();
		}
		if (IsValid(InteriorDesignWidget) && InteriorDesignWidget->IsInViewport()) {
			InteriorDesignWidget->RemoveFromParent();
		}
		if (IsValid(RoadConstructionWidget) && RoadConstructionWidget->IsInViewport()) {
			RoadConstructionWidget->RemoveFromParent();
		}
		break;
	case EArchVizMode::RoadConstruction:
		if (IsValid(BuildingCreationWidget) && BuildingCreationWidget->IsInViewport()) {
			BuildingCreationWidget->RemoveFromParent();
		}
		if (IsValid(InteriorDesignWidget) && InteriorDesignWidget->IsInViewport()) {
			InteriorDesignWidget->RemoveFromParent();
		}
		if (!RoadConstructionWidget->IsInViewport()) {
			RoadConstructionWidget->AddToViewport();
		}
		break;
	case EArchVizMode::BuildingCreation:
		if (!BuildingCreationWidget->IsInViewport()) {
			BuildingCreationWidget->AddToViewport();
		}
		if (IsValid(InteriorDesignWidget) && InteriorDesignWidget->IsInViewport()) {
			InteriorDesignWidget->RemoveFromParent();
		}
		if (IsValid(RoadConstructionWidget) && RoadConstructionWidget->IsInViewport()) {
			RoadConstructionWidget->RemoveFromParent();
		}
		break;
	case EArchVizMode::InteriorDesign:
		if (IsValid(BuildingCreationWidget) && BuildingCreationWidget->IsInViewport()) {
			BuildingCreationWidget->RemoveFromParent();
		}
		if (!InteriorDesignWidget->IsInViewport()) {
			InteriorDesignWidget->AddToViewport();
		}
		if (IsValid(RoadConstructionWidget) && RoadConstructionWidget->IsInViewport()) {
			RoadConstructionWidget->RemoveFromParent();
		}
		break;
	default:
		break;
	}
}