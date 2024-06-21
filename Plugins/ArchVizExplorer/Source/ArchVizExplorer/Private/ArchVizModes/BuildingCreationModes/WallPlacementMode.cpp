// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/WallPlacementMode.h"
#include "Actors/BuildingCreation/WallActor.h"
#include "Utilities/ArchVizUtility.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"

UWallPlacementMode::UWallPlacementMode() : PlayerController{ nullptr }, InputMappingContext{ nullptr }, WallActor{ nullptr } {}

void UWallPlacementMode::SetupInputMapping() {
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	check(EnhancedInputComponent);

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

void UWallPlacementMode::InitParam(APlayerController* Controller) {
	PlayerController = Controller;
}

void UWallPlacementMode::PreviewSegment() {
	if (IsValid(WallActorRef) && !IsValid(WallActor)) {
		WallActor = NewObject<AWallActor>(this, WallActorRef);
	}
	if (IsValid(WallActor) && !IsValid(WallActor->PreviewWallSegment)) {
		WallActor->PreviewWallSegment = NewObject<UStaticMeshComponent>();
		WallActor->PreviewWallSegment->RegisterComponentWithWorld(GetWorld());
	}

	FHitResult HitResult = WallActor->GetHitResult();

	if (IsValid(WallActor->WallMesh) && IsValid(WallActor->PreviewWallSegment)) {
		WallActor->PreviewWallSegment->SetStaticMesh(WallActor->WallMesh);

		WallActor->PreviewWallSegment->SetWorldLocation(ArchVizUtility::SnapToGrid(HitResult.Location));
		WallActor->PreviewWallSegment->SetWorldRotation(WallActor->GetSegmentRotation());
	}
}

void UWallPlacementMode::CleanUp() {
	if (IsValid(WallActor)) {
		WallActor->DestroyPreviewWallSegment();
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

FHitResult UWallPlacementMode::GetHitResult() const {
	FHitResult MouseHitResult{};

	FVector WorldLocation{}, WorldDirection{};

	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.AddIgnoredActor(WallActor);
		CollisionQueryParams.AddIgnoredActor(PlayerController->GetPawn());

		GetWorld()->LineTraceSingleByChannel(MouseHitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);

		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.0f);
		if (MouseHitResult.bBlockingHit) {
			DrawDebugPoint(GetWorld(), MouseHitResult.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);
		}
	}

	if(GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, MouseHitResult.ImpactPoint.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, MouseHitResult.Location.ToString());
	}

	return MouseHitResult;
}