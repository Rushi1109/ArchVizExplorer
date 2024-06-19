// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "RoadActor.h"
#include "WallActor.h"

AArchVizController::AArchVizController() : CurrentMode{ nullptr }, ArchVizMode{ EArchVizMode::ViewMode }, BuildingModeEntity{ EBuildingModeEntity::None } {

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
		}

		if (IsValid(BuildingCreationWidgetRef)) {
			BuildingCreationWidget = CreateWidget<UBuildingCreationWidget>(this, BuildingCreationWidgetRef, TEXT("Building Creation Widget"));
			BuildingCreationWidget->OnBuildingModeEntityChange.AddUObject(this, &AArchVizController::HandleBuildingModeEntityChange);
		}

		if (IsValid(InteriorDesignWidgetRef)) {
			InteriorDesignWidget = CreateWidget<UInteriorDesignWidget>(this, InteriorDesignWidgetRef, TEXT("Interior Design Widget"));
		}
	}

	if (IsValid(RoadConstructionModeRef)) {
		RoadConstructionMode = NewObject<URoadConstructionMode>(this, RoadConstructionModeRef);
		RoadConstructionMode->InitParam(this);
		RoadConstructionMode->SetupInputMapping();
	}
	if (IsValid(BuildingCreationModeRef)) {
		BuildingCreationMode = NewObject<UBuildingCreationMode>(this, BuildingCreationModeRef);
		BuildingCreationMode->InitParam(this);
		BuildingCreationMode->SetBuildingModeEntity(BuildingModeEntity);
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

}

void AArchVizController::HandleModeChange(EArchVizMode NewArchVizMode) {
	ArchVizMode = NewArchVizMode;

	switch (ArchVizMode) {
	case EArchVizMode::ViewMode:
		break;
	case EArchVizMode::RoadConstruction:
		SetArchVizMode(RoadConstructionMode);
		break;
	case EArchVizMode::BuildingCreation:
		SetArchVizMode(BuildingCreationMode);
		break;
	case EArchVizMode::InteriorDesign:
		break;
	default:
		break;
	}

	//CleanBeforeChange();
	UpdateWidgets();
}

void AArchVizController::SetArchVizMode(IArchVizMode* NewArchVizMode) {
	if (CurrentMode) {
		CurrentMode->ExitMode();
	}

	CurrentMode = NewArchVizMode;

	if (CurrentMode) {
		CurrentMode->EnterMode();
	}
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
		WallSegment->SetWorldScale3D(FVector{ 1.0 + (WallActor->WallMesh->GetBoundingBox().GetSize().Y / WallActor->WallMesh->GetBoundingBox().GetSize().X), 1.0, 1.0 });

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