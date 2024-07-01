// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/WallActor.h"
#include "ProceduralMeshComponent.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Utilities/ArchVizUtility.h"
#include "Actors/BuildingCreation/DoorActor.h"
#include "Widgets/PropertyPanelWidget.h"

// Sets default values
AWallActor::AWallActor() : WallMesh{nullptr}, DoorAttachableWallMesh{nullptr} {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(PropertyPanelRef)) {
		PropertyPanel = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelRef);
		PropertyPanel->WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

// Called every frame
void AWallActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (State == EBuildingActorState::Previewing) {
		HandlePreviewingState();
	}
	else if (State == EBuildingActorState::Moving) {
		HandleMovingState();
	}
	else if (State == EBuildingActorState::Generating) {
		HandleGeneratingState();
	}
}

void AWallActor::GenerateSegments(double InLength /*= 0.0*/) {
	SetLength(InLength);

	DestroySegments();

	int32 NumberOfSegments{};

	FVector WallBounds{};
	if (IsValid(WallMesh)) {
		WallBounds = WallMesh->GetBoundingBox().GetSize();
		NumberOfSegments = FMath::FloorToInt32(Length / WallBounds.X) + 1;
	}

	for (int32 SegmentIndex = 0; SegmentIndex < NumberOfSegments; ++SegmentIndex) {
		UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		StaticMeshComponent->RegisterComponentWithWorld(GetWorld());
		StaticMeshComponent->SetRelativeLocation(FVector{ SegmentIndex * WallBounds.X, 0, 0 });

		if (IsValid(WallMesh)) {
			StaticMeshComponent->SetStaticMesh(WallMesh);
			WallSegments.Add(StaticMeshComponent);
		}
	}

	UpdateSegments();
}

void AWallActor::DestroySegments() {
	for (auto& WallSegment : WallSegments) {
		WallSegment->DestroyComponent();
		WallSegment = nullptr;
	}

	WallSegments.Empty();
}

void AWallActor::UpdateSegments() {
	int32 NumberOfSegments = WallSegments.Num();

	for (auto It = IndexDoorMapping.CreateIterator(); It; ++It) {
		int32 SegmentIndex = It.Key();
		ADoorActor* DoorActor = It.Value();

		if (SegmentIndex > (NumberOfSegments - 1)) {
			if (IsValid(DoorActor)) {
				DoorActor->Destroy();
			}
			It.RemoveCurrent();
			continue;
		}

		if (WallSegments.IsValidIndex(SegmentIndex)) {
			WallSegments[SegmentIndex]->SetStaticMesh(DoorAttachableWallMesh);

			DoorActor->AttachToComponent(WallSegments[SegmentIndex], FAttachmentTransformRules::KeepRelativeTransform, TEXT("DoorSocket"));
			DoorActor->SetActorRelativeRotation(FRotator::ZeroRotator);
			DoorActor->SetActorRelativeLocation(FVector::ZeroVector);
		}
	}
}

void AWallActor::HandlePreviewingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void AWallActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	SetActorLocation(HitResult.Location);
	SetStartLocation(HitResult.Location);

	AdjustEdgeOffset();
}

void AWallActor::HandleGeneratingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	SetEndLocation(HitResult.Location);

	double XWallLength = EndLocation.X - StartLocation.X;
	double YWallLength = EndLocation.Y - StartLocation.Y;

	if (EndLocation != StartLocation) { 
		if (abs(XWallLength) > abs(YWallLength)) {
			GenerateSegments(abs(XWallLength));

			if (XWallLength >= 0) {
				SetActorRotation(FRotator{0.0, 0.0, 0.0});
			}
			else {
				SetActorRotation(FRotator{0.0, 180.0, 0.0});
			}
		}
		else {
			GenerateSegments(abs(YWallLength));

			if (YWallLength >= 0) {
				SetActorRotation(FRotator{ 0.0, 90.0, 0.0 });
			}
			else {
				SetActorRotation(FRotator{ 0.0, 270.0, 0.0 });
			}
		}

		AdjustEdgeOffset();
	}
}

void AWallActor::AdjustEdgeOffset() {
	FRotator ActorRotation = GetActorRotation();

	if (ActorRotation.Yaw >= -5.0 && ActorRotation.Yaw <= 5.0) {
		SetActorLocation(FVector{StartLocation.X + 10, StartLocation.Y, StartLocation.Z});
	}
	else if (ActorRotation.Yaw >= 85.0 && ActorRotation.Yaw <= 95.0) {
		SetActorLocation(FVector{ StartLocation.X, StartLocation.Y + 10, StartLocation.Z });
	}
	else if (ActorRotation.Yaw >= 175.0 && ActorRotation.Yaw <= 185.0) {
		SetActorLocation(FVector{ StartLocation.X - 10, StartLocation.Y, StartLocation.Z });
	}
	else if (ActorRotation.Yaw >= -95.0 && ActorRotation.Yaw <= -85.0) {
		SetActorLocation(FVector{ StartLocation.X, StartLocation.Y - 10, StartLocation.Z });
	}
}

void AWallActor::UpdateWallLenghSlider() {
	if (IsValid(PropertyPanel)) {
		double XDistance = FMath::Abs(EndLocation.X - StartLocation.X);
		double YDistance = FMath::Abs(EndLocation.Y - StartLocation.Y);

		if (XDistance >= YDistance) {
			PropertyPanel->WallLengthSpinBox->SetValue(XDistance);
		}
		else {
			PropertyPanel->WallLengthSpinBox->SetValue(YDistance);
		}
	}
}

void AWallActor::AttachDoorComponent(UPrimitiveComponent* ComponentToReplace, ADoorActor* DoorActor) {
	if (UStaticMeshComponent* SegmentStaticMesh = Cast<UStaticMeshComponent>(ComponentToReplace)) {
		int32 SegmentIndex = WallSegments.Find(SegmentStaticMesh);

		if (SegmentIndex != INDEX_NONE) {
			DoorActor->ParentWallComponentIndex = SegmentIndex;
			WallSegments[SegmentIndex]->SetStaticMesh(DoorAttachableWallMesh);
			IndexDoorMapping.Add(TTuple<int32, ADoorActor*>{SegmentIndex, DoorActor});

			DoorActor->AttachToComponent(WallSegments[SegmentIndex], FAttachmentTransformRules::KeepRelativeTransform, TEXT("DoorSocket"));
			DoorActor->SetActorRelativeRotation(FRotator::ZeroRotator);
			DoorActor->SetActorRelativeLocation(FVector::ZeroVector);
		}
	}
}

void AWallActor::DetachDoorComponent(ADoorActor* DoorActor) {
	if (IsValid(DoorActor) && IsValid(WallMesh)) {
		if (const int32* SegmentIndex = IndexDoorMapping.FindKey(DoorActor)) {
				WallSegments[*SegmentIndex]->SetStaticMesh(WallMesh);
				DoorActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

				IndexDoorMapping.Remove(*SegmentIndex);
		}
	}
}

void AWallActor::DestroyDoorComponents() {
	for (auto& [SegmentIndex, DoorActor] : IndexDoorMapping) {
		if (IsValid(DoorActor)) {
			DoorActor->Destroy();
		}
	}

	IndexDoorMapping.Empty();
}

double AWallActor::GetLength() const {
	return Length;
}

void AWallActor::SetLength(double InValue) {
	Length = InValue;
}

void AWallActor::SetStartLocation(const FVector& NewStartLocation) {
	StartLocation = NewStartLocation;
}

const FVector& AWallActor::GetStartLocation() const {
	return StartLocation;
}

void AWallActor::SetEndLocation(const FVector& NewEndLocation) {
	EndLocation = NewEndLocation;
}

const FVector& AWallActor::GetEndLocation() const {
	return EndLocation;
}