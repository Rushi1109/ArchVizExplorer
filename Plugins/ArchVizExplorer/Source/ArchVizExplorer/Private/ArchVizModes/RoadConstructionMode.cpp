// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/RoadConstructionMode.h"
#include "Actors/RoadActor.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Widgets/PropertyPanelWidget.h"
#include "Components/SplineComponent.h"

URoadConstructionMode::URoadConstructionMode() : RoadActor{ nullptr } {}

void URoadConstructionMode::Setup() {
	if (IsValid(RoadActorRef) && !IsValid(RoadActor)) {
		RoadActor = GetWorld()->SpawnActor<ARoadActor>(RoadActorRef, FTransform{});
		RoadActor->SetState(ERoadActorState::Selected);

		BindWidgetDelegates();
	}

	if (IsValid(WidgetRef) && !IsValid(Widget)) {
		Widget = CreateWidget<URoadConstructionWidget>(GetWorld(), WidgetRef, "Road Mode Widget");
	}
}

void URoadConstructionMode::Cleanup() {
	if (IsValid(RoadActor)) {
		RoadActor->SetState(ERoadActorState::None);
		RoadActor = nullptr;
	}
}

void URoadConstructionMode::SetupInputMapping() {
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

		UInputAction* LeftClickAction = NewObject<UInputAction>(this);
		LeftClickAction->ValueType = EInputActionValueType::Boolean;

		UInputAction* NKeyPressAction = NewObject<UInputAction>(this);
		NKeyPressAction->ValueType = EInputActionValueType::Boolean;

		UInputAction* DeleteKeyPressAction = NewObject<UInputAction>(this);
		DeleteKeyPressAction->ValueType = EInputActionValueType::Boolean;

		InputMappingContext = NewObject<UInputMappingContext>(this);
		InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);
		InputMappingContext->MapKey(NKeyPressAction, EKeys::N);
		InputMappingContext->MapKey(DeleteKeyPressAction, EKeys::Delete);

		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleLeftClickAction);
		EnhancedInputComponent->BindAction(NKeyPressAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleNKeyPressAction);
		EnhancedInputComponent->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleDeleteKeyPressAction);
	}
}

void URoadConstructionMode::EnterMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		ShowWidget();
		Setup();
	}
}

void URoadConstructionMode::ExitMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
		}

		HideWidget();
		Cleanup();
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

void URoadConstructionMode::HandleNKeyPressAction() {
	Cleanup();

	Setup();
}

void URoadConstructionMode::HandleDeleteKeyPressAction() {
	if (IsValid(RoadActor)) {
		RoadActor->SetState(ERoadActorState::None);
		RoadActor->SplineComponent->ClearSplinePoints();
		RoadActor->DestroyActor();

		RoadActor = nullptr;
	}
}

void URoadConstructionMode::BindWidgetDelegates() {
	if (IsValid(RoadActor) && IsValid(RoadActor->PropertyPanel)) {
		RoadActor->PropertyPanel->NewRoadButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleNewButtonClick);
		RoadActor->PropertyPanel->DeleteRoadButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleDeleteButtonClick);
		RoadActor->PropertyPanel->ClosePanelRoadButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleClosePanelButtonClick);
		RoadActor->PropertyPanel->RoadTypeComboBox->OnSelectionChanged.AddDynamic(this, &URoadConstructionMode::HandleRoadTypeChange);
	}
}

void URoadConstructionMode::HandleNewButtonClick() {
	HandleNKeyPressAction();
}

void URoadConstructionMode::HandleDeleteButtonClick() {
	HandleDeleteKeyPressAction();
}

void URoadConstructionMode::HandleClosePanelButtonClick() {
	Cleanup();
}

void URoadConstructionMode::HandleRoadTypeChange(FString Selectedtype, ESelectInfo::Type SelectionType) {
	if (Selectedtype == TEXT("Curved")) {
		RoadActor->SetPointType(ERoadPointType::Curved);
	}
	else if (Selectedtype == TEXT("Sharp")) {
		RoadActor->SetPointType(ERoadPointType::Sharp);
	}

	RoadActor->UpdateRoad();
}
