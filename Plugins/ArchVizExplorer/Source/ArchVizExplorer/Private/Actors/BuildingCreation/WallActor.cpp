// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/WallActor.h"
#include "ProceduralMeshComponent.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Utilities/ArchVizUtility.h"

// Sets default values
AWallActor::AWallActor() : WallMesh{nullptr}, State{EWallActorState::Selected} {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	PrimaryActorTick.TickInterval = 0.3;
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay() {
	Super::BeginPlay();

	if (PropertyPanelRef) {
		PropertyPanel = CreateWidget<UUserWidget>(GetWorld(), PropertyPanelRef);
		PropertyPanel->AddToViewport();
	}
}

// Called every frame
void AWallActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (State == EWallActorState::Previewing) {
		HandlePreviewingState();
	}
	else if (State == EWallActorState::Moving) {
		HandleMovingState();
	}
	else if (State == EWallActorState::Generating) {
		HandleGeneratingState();
	}
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

void AWallActor::SetState(EWallActorState NewState) {
	State = NewState;
}

EWallActorState AWallActor::GetState() const {
	return State;
}

void AWallActor::GenerateSegments(double Length /*= 0.0*/) {
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
}

void AWallActor::DestroySegments() {
	for (auto& WallSegment : WallSegments) {
		WallSegment->DestroyComponent();
		WallSegment = nullptr;
	}

	WallSegments.Empty();
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
}

void AWallActor::HandleGeneratingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);;

	SetEndLocation(HitResult.Location);

	double XWallLength = EndLocation.X - StartLocation.X;
	double YWallLength = EndLocation.Y - StartLocation.Y;

	if (EndLocation != StartLocation) { 
		if (abs(XWallLength) > abs(YWallLength)) {
			GenerateSegments(abs(XWallLength));

			if (XWallLength >= 0) {
				SetActorRotation(FRotator{0.0, 0.0, 0.0});
				SetActorLocation(FVector{ StartLocation.X + 10, StartLocation.Y, StartLocation.Z });
			}
			else {
				SetActorRotation(FRotator{0.0, 180.0, 0.0});
				SetActorLocation(FVector{ StartLocation.X - 10, StartLocation.Y, StartLocation.Z });
			}
		}
		else {
			GenerateSegments(abs(YWallLength));

			if (YWallLength >= 0) {
				SetActorRotation(FRotator{ 0.0, 90.0, 0.0 });
				SetActorLocation(FVector{ StartLocation.X, StartLocation.Y + 10, StartLocation.Z });
			}
			else {
				SetActorRotation(FRotator{ 0.0, 270.0, 0.0 });
				SetActorLocation(FVector{ StartLocation.X, StartLocation.Y - 10, StartLocation.Z });
			}
		}
	}
}