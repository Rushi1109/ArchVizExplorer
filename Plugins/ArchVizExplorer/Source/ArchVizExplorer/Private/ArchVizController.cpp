// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "RoadActor.h"
#include "WallActor.h"

AArchVizController::AArchVizController() : ArchVizMode{ EArchVizMode::ViewMode }, RoadGeneratorMappingContext{ nullptr }, WallGeneratorMappingContext{ nullptr } {
	
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

		if (IsValid(BuildingCreationWidgetRef)) {
			BuildingCreationWidget = CreateWidget<UBuildingCreationWidget>(this, BuildingCreationWidgetRef, TEXT("Building Creation Widget"));
			BuildingCreationWidget->AddToViewport(0);
			BuildingCreationWidget->OnBuildingModeEntityChange.AddUObject(this, &AArchVizController::HandleBuildingModeEntityChange);
		}

		if (IsValid(InteriorDesignWidgetRef)) {
			InteriorDesignWidget = CreateWidget<UInteriorDesignWidget>(this, InteriorDesignWidgetRef, TEXT("Interior Design Widget"));
			InteriorDesignWidget->AddToViewport(0);
		}
	}
}

void AArchVizController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (ArchVizMode == EArchVizMode::BuildingCreation) {
		if (BuildingModeEntity == EBuildingModeEntity::WallPlacement) {
			if (IsValid(WallActorRef) && !IsValid(WallActor)) {
				WallActor = NewObject<AWallActor>(this, WallActorRef);
			}
			if (IsValid(WallActor) && !IsValid(WallActor->PreviewWallSegment)) {
				WallActor->PreviewWallSegment = NewObject<UStaticMeshComponent>();
				WallActor->PreviewWallSegment->RegisterComponentWithWorld(GetWorld());
			}
			FHitResult HitResult = GetHitResult();
			//HitResult.Location.Z = 0.0;

			if (IsValid(WallActor->WallMesh) && IsValid(WallActor->PreviewWallSegment)) {
				WallActor->PreviewWallSegment->SetStaticMesh(WallActor->WallMesh);

				WallActor->PreviewWallSegment->SetWorldLocation(SnapToGrid(HitResult.Location));
				WallActor->PreviewWallSegment->SetWorldRotation(WallActor->GetSegmentRotation());
			}
		}
	}
}

FVector AArchVizController::SnapToGrid(const FVector& WorldLocation) {
	float GridSize = 100.0f;
	float SnappedX = FMath::RoundToFloat(WorldLocation.X / GridSize) * GridSize;
	float SnappedY = FMath::RoundToFloat(WorldLocation.Y / GridSize) * GridSize;
	float SnappedZ = FMath::RoundToFloat(WorldLocation.Z / GridSize) * GridSize;
	return FVector(SnappedX, SnappedY, SnappedZ);
}

void AArchVizController::SetupInputComponent() {
	Super::SetupInputComponent();

	SetupRoadGeneratorInput();
	SetupWallGeneratorInput();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(RoadGeneratorMappingContext, 0);
	}
}

void AArchVizController::HandleModeChange(EArchVizMode NewArchVizMode) {
	ArchVizMode = NewArchVizMode;

	CleanBeforeChange();
	UpdateMappingContext();
	UpdateWidgets();
}

void AArchVizController::HandleBuildingModeEntityChange(EBuildingModeEntity NewBuildingModeEntity) {
	BuildingModeEntity = NewBuildingModeEntity;
}

FHitResult AArchVizController::GetHitResult() const {
	FHitResult MouseHitResult{};

	FVector WorldLocation{}, WorldDirection{};

	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.AddIgnoredActor(WallActor);

		GetWorld()->LineTraceSingleByChannel(MouseHitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);	
	}

	return MouseHitResult;
}

void AArchVizController::SetupRoadGeneratorInput() {
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);

	UInputAction* RoadGeneratorLeftClickAction = NewObject<UInputAction>(this);
	RoadGeneratorLeftClickAction->ValueType = EInputActionValueType::Boolean;

	RoadGeneratorMappingContext = NewObject<UInputMappingContext>(this);
	RoadGeneratorMappingContext->MapKey(RoadGeneratorLeftClickAction, EKeys::LeftMouseButton);

	EnhancedInputComponent->BindAction(RoadGeneratorLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::HandleRoadGeneratorLeftClick);
}

void AArchVizController::HandleRoadGeneratorLeftClick() {
	FHitResult HitResult = GetHitResult();

	if (IsValid(RoadActorRef) && !IsValid(RoadActor)) {
		RoadActor = NewObject<ARoadActor>(this, RoadActorRef);
	}

	RoadActor->AddNewPoint(HitResult.Location);
}

void AArchVizController::SetupWallGeneratorInput() {
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);

	UInputAction* WallGeneratorLeftClickAction = NewObject<UInputAction>(this);
	WallGeneratorLeftClickAction->ValueType = EInputActionValueType::Boolean;

	UInputAction* WallGeneratorRKeyPressAction = NewObject<UInputAction>(this);
	WallGeneratorRKeyPressAction->ValueType = EInputActionValueType::Boolean;

	WallGeneratorMappingContext = NewObject<UInputMappingContext>(this);
	WallGeneratorMappingContext->MapKey(WallGeneratorLeftClickAction, EKeys::LeftMouseButton);
	WallGeneratorMappingContext->MapKey(WallGeneratorRKeyPressAction, EKeys::R);

	EnhancedInputComponent->BindAction(WallGeneratorLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::HandleWallGeneratorLeftClick);
	EnhancedInputComponent->BindAction(WallGeneratorRKeyPressAction, ETriggerEvent::Completed, this, &AArchVizController::HandleWallGeneratorRKeyPress);
}

void AArchVizController::HandleWallGeneratorLeftClick() {

	auto WallSegment = NewObject<UStaticMeshComponent>();
	
	if (IsValid(WallActor->WallMesh)) {
		WallSegment->SetStaticMesh(WallActor->WallMesh);

		WallSegment->RegisterComponentWithWorld(GetWorld());
		WallSegment->SetWorldLocation(WallActor->PreviewWallSegment->GetComponentLocation());
		WallSegment->SetWorldRotation(WallActor->GetSegmentRotation());
		WallSegment->SetWorldScale3D(FVector{ 1.0 + (WallActor->WallMesh->GetBoundingBox().GetSize().Y / WallActor->WallMesh->GetBoundingBox().GetSize().X), 1.0, 1.0});

		WallActor->WallSegments.Add(WallSegment);
		WallActor->SetSegmentIndex(WallActor->GetSegmentIndex() + 1);
	}
}

void AArchVizController::HandleWallGeneratorRKeyPress() {
	if (IsValid(WallActor)) {
		double NewRotationYaw = (WallActor->GetSegmentRotation().Yaw + 90);
		if (NewRotationYaw >= 360) {
			NewRotationYaw -= 360;
		}
		WallActor->SetSegmentRotation(FRotator{ 0.0, NewRotationYaw, 0.0 });
	}
}

void AArchVizController::CleanBeforeChange() {
	if (IsValid(WallActor)) {
		WallActor->DestroyPreviewWallSegment();
	}

	BuildingModeEntity = EBuildingModeEntity::None;
}

void AArchVizController::UpdateMappingContext() {
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	switch (ArchVizMode) {
	case EArchVizMode::ViewMode:

		break;
	case EArchVizMode::RoadConstruction:
		Subsystem->AddMappingContext(RoadGeneratorMappingContext, 0);
		Subsystem->RemoveMappingContext(WallGeneratorMappingContext);
		break;
	case EArchVizMode::BuildingCreation:
		Subsystem->AddMappingContext(WallGeneratorMappingContext, 0);
		Subsystem->RemoveMappingContext(RoadGeneratorMappingContext);
		break;
	case EArchVizMode::InteriorDesign:
		Subsystem->RemoveMappingContext(WallGeneratorMappingContext);
		Subsystem->RemoveMappingContext(RoadGeneratorMappingContext);
		break;
	}
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
	}
}