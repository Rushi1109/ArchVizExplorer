// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/RoadConstructionMode.h"
#include "ArchVizController.h"
#include "Actors/RoadActor.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Widgets/PropertyPanelWidget.h"
#include "Components/SplineComponent.h"

URoadConstructionMode::URoadConstructionMode() : RoadActor{ nullptr } {}

void URoadConstructionMode::Setup() {
	RoadModeState = ERoadModeState::Free;

	if (IsValid(WidgetRef) && !IsValid(Widget)) {
		Widget = CreateWidget<URoadConstructionWidget>(GetWorld(), WidgetRef, "Road Mode Widget");

		if (URoadConstructionWidget* RoadConstructionWidget = Cast<URoadConstructionWidget>(Widget)) {
			RoadConstructionWidget->NewRoadSegmentButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleNewSegmentButtonClick);
			RoadConstructionWidget->CompleteCurrentSegmentButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleCompleteSegmentButtonClick);
			RoadConstructionWidget->UndoButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleUndoButtonClick);
		}
	}
}

void URoadConstructionMode::Cleanup() {
	if (IsValid(RoadActor)) {
		RoadActor->SetState(ERoadActorState::None);
		RoadActor = nullptr;
		RoadModeState = ERoadModeState::Free;
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

		UInputAction* ZKeyPressAction = NewObject<UInputAction>(this);
		ZKeyPressAction->ValueType = EInputActionValueType::Boolean;

		InputMappingContext = NewObject<UInputMappingContext>(this);
		InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);
		InputMappingContext->MapKey(NKeyPressAction, EKeys::N);
		InputMappingContext->MapKey(DeleteKeyPressAction, EKeys::Delete);
		InputMappingContext->MapKey(ZKeyPressAction, EKeys::Z);

		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleLeftClickAction);
		EnhancedInputComponent->BindAction(NKeyPressAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleNKeyPressAction);
		EnhancedInputComponent->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleDeleteKeyPressAction);
		EnhancedInputComponent->BindAction(ZKeyPressAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleZKeyPressAction);
	}
}

void URoadConstructionMode::EnterMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		ShowWidget();
		Setup();

		PlayerController->SetSuccess(FText::FromString("Switched To Road Construction Mode."));
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

void URoadConstructionMode::HandleNewSegmentButtonClick() {
	HandleNKeyPressAction();
}

void URoadConstructionMode::HandleCompleteSegmentButtonClick() {
	if (IsValid(RoadActor)) {
		RoadActor->SetState(ERoadActorState::Selected);
		RoadModeState = ERoadModeState::Free;

		PlayerController->SetSuccess(FText::FromString("Successfully Completed Road Segment."));
	}
}

void URoadConstructionMode::HandleUndoButtonClick() {
	HandleZKeyPressAction();
}

void URoadConstructionMode::HandleLeftClickAction() {
	switch (RoadModeState) {
	case ERoadModeState::Free:
		HandleFreeState();
		break;
	case ERoadModeState::OldEntity:
		[[fallthrough]];
	case ERoadModeState::NewEntity:
		HandleNewEntityState();
		break;
	}
}

void URoadConstructionMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();

	if (IsValid(RoadActor)) {
		RoadActor->SetState(ERoadActorState::None);
		RoadActor = nullptr;
	}

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ARoadActor::StaticClass())) {
		RoadActor = Cast<ARoadActor>(HitResult.GetActor());
		RoadActor->SetState(ERoadActorState::Selected);
		RoadModeState = ERoadModeState::Free;
	}
}

void URoadConstructionMode::HandleOldEntityState() {
	
}

void URoadConstructionMode::HandleNewEntityState() {
	if (IsValid(RoadActor)) {
		FHitResult HitResult{};

		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(RoadActor);

		HitResult = RoadActor->GetHitResult(IgnoredActors);
		RoadActor->AddNewPoint(HitResult.Location);
	}
	else {
		PlayerController->SetError(FText::FromString("Create New Road Segment First."));
	}
}

void URoadConstructionMode::HandleNKeyPressAction() {
	Cleanup();

	Setup();

	if (IsValid(RoadActorRef) && !IsValid(RoadActor)) {
		RoadActor = GetWorld()->SpawnActor<ARoadActor>(RoadActorRef, FTransform{});
		RoadActor->SetState(ERoadActorState::Generating);
		RoadModeState = ERoadModeState::NewEntity;

		BindWidgetDelegates();

		PlayerController->SetSuccess(FText::FromString("New Road Segment Started."));
	}

}

void URoadConstructionMode::HandleDeleteKeyPressAction() {
	if (IsValid(RoadActor)) {
		RoadActor->SetState(ERoadActorState::None);
		RoadActor->SplineComponent->ClearSplinePoints();
		RoadActor->DestroyActor();
		RoadModeState = ERoadModeState::Free;

		RoadActor = nullptr;

		PlayerController->SetSuccess(FText::FromString("Deleted Road Segment."));
	}
}

void URoadConstructionMode::HandleZKeyPressAction() {
	if (IsValid(RoadActor)) {
		RoadActor->SetState(ERoadActorState::Generating);
		RoadActor->RemoveLastSplinePoint();
		RoadModeState = ERoadModeState::OldEntity;
	}
}

void URoadConstructionMode::BindWidgetDelegates() {
	if (IsValid(RoadActor) && IsValid(RoadActor->PropertyPanel)) {
		RoadActor->PropertyPanel->NewRoadButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleNewButtonClick);
		RoadActor->PropertyPanel->DeleteRoadButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleDeleteButtonClick);
		RoadActor->PropertyPanel->ClosePanelRoadButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleClosePanelButtonClick);
		RoadActor->PropertyPanel->RoadTypeComboBox->OnSelectionChanged.AddDynamic(this, &URoadConstructionMode::HandleRoadTypeChange);
		RoadActor->PropertyPanel->RoadWidthSpinBox->OnValueChanged.AddDynamic(this, &URoadConstructionMode::HandleRoadWidthChange);
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
}

void URoadConstructionMode::HandleRoadWidthChange(float InWidth) {
	if (IsValid(RoadActor)) {
		RoadActor->Width = InWidth;

		RoadActor->UpdateRoad();
	}
}
