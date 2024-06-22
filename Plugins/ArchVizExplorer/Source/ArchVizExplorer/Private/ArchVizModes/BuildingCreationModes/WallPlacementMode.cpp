// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/WallPlacementMode.h"
#include "Actors/BuildingCreation/WallActor.h"
#include "Utilities/ArchVizUtility.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"

UWallPlacementMode::UWallPlacementMode() : WallActor{ nullptr } {}

void UWallPlacementMode::Setup() {
	if (WallActorRef) {
		WallActor = GetWorld()->SpawnActor<AWallActor>(WallActorRef);
	}
}

void UWallPlacementMode::SetupInputMapping() {
	if (IsValid(PlayerController)) {
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

		UInputAction* LeftClickAction = NewObject<UInputAction>(this);
		LeftClickAction->ValueType = EInputActionValueType::Boolean;

		UInputAction* RKeyPressAction = NewObject<UInputAction>(this);
		RKeyPressAction->ValueType = EInputActionValueType::Boolean;

		InputMappingContext = NewObject<UInputMappingContext>(this);
		InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);
		InputMappingContext->MapKey(RKeyPressAction, EKeys::R);

		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleLeftClickAction);
		EnhancedInputComponent->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UWallPlacementMode::HandleRKeyPressAction);
	}
}

void UWallPlacementMode::EnterSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void UWallPlacementMode::ExitSubMode() {
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
		}
	}
}

void UWallPlacementMode::HandleLeftClickAction() {
	if (IsValid(WallActor) && IsValid(WallActor->WallMesh) && IsValid(WallActor->PreviewWallSegment) && (WallActor->PreviewWallSegment->GetComponentLocation() != FVector{0.0, 0.0, 0.0})) {
		auto WallSegment = NewObject<UStaticMeshComponent>();
		WallSegment->SetStaticMesh(WallActor->WallMesh);

		WallSegment->RegisterComponentWithWorld(GetWorld());
		WallSegment->SetWorldLocation(WallActor->PreviewWallSegment->GetComponentLocation());
		WallSegment->SetWorldRotation(WallActor->GetSegmentRotation());
		WallSegment->SetWorldScale3D(FVector{ 1.0 + (WallActor->WallMesh->GetBoundingBox().GetSize().Y / WallActor->WallMesh->GetBoundingBox().GetSize().X), 1.0, 1.0 });

		WallActor->WallSegments.Add(WallSegment);
		WallActor->SetSegmentIndex(WallActor->GetSegmentIndex() + 1);
	}
}

void UWallPlacementMode::HandleRKeyPressAction() {
	if (IsValid(WallActor)) {
		double NewRotationYaw = (WallActor->GetSegmentRotation().Yaw + 90);
		if (NewRotationYaw >= 360) {
			NewRotationYaw -= 360;
		}
		WallActor->SetSegmentRotation(FRotator{ 0.0, NewRotationYaw, 0.0 });
	}
}