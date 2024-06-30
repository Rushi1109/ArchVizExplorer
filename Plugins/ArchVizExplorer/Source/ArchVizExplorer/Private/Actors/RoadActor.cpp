// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RoadActor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARoadActor::ARoadActor() : RoadActorState{ERoadActorState::None}, RoadPointType{ERoadPointType::Sharp}, RoadMesh{ nullptr }, Width{400.f} {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("RoadSpline"));
	SplineComponent->SetupAttachment(SceneRoot);

	SplineComponent->ClearSplinePoints(true);
}

// Called when the game starts or when spawned
void ARoadActor::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(PropertyPanelRef)) {
		PropertyPanel = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelRef);
		PropertyPanel->WidgetSwitcher->SetActiveWidgetIndex(5);
	}
}

// Called every frame
void ARoadActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ARoadActor::HandleStateChange() {
	if (RoadActorState == ERoadActorState::Selected) {
		ShowPropertyPanel();
		HighlightSelectedActor();
	}
	else {
		HidePropertyPanel();
		UnHighlightDeselectedActor();
	}
}

void ARoadActor::GenerateRoadMesh() {
	DestroyRoadMesh();

	if (!IsValid(SplineComponent) || !IsValid(RoadMesh)) return;

	int32 SplinePointsCount = SplineComponent->GetNumberOfSplinePoints();
	if (SplinePointsCount < 2) {
		return;
	}

	double SplineLength = SplineComponent->GetSplineLength();

	double RoadMeshSizeX = 1.0;
	if (IsValid(RoadMesh)) {
		RoadMeshSizeX = ((RoadMesh->GetBoundingBox().Max - RoadMesh->GetBoundingBox().Min).X / 4);
	}

	int32 InstanceCount = FMath::FloorToInt(SplineLength / RoadMeshSizeX);

	for (int32 i = 0; i <= InstanceCount; ++i) {
		FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(i * RoadMeshSizeX, ESplineCoordinateSpace::Local);
		FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(i * RoadMeshSizeX, ESplineCoordinateSpace::Local);

		FVector EndLocation = SplineComponent->GetLocationAtDistanceAlongSpline(FMath::Min((i + 1) * RoadMeshSizeX, SplineLength), ESplineCoordinateSpace::Local);
		FVector EndTangent = SplineComponent->GetTangentAtDistanceAlongSpline(FMath::Min((i + 1) * RoadMeshSizeX, SplineLength), ESplineCoordinateSpace::Local);

		StartTangent = UKismetMathLibrary::ClampVectorSize(StartTangent, 0.0, RoadMeshSizeX);
		EndTangent = UKismetMathLibrary::ClampVectorSize(EndTangent, 0.0, RoadMeshSizeX);

		GenerateRoadSegment(StartLocation, StartTangent, EndLocation, EndTangent);
	}
}

void ARoadActor::GenerateRoadSegment(const FVector& StartLocation, const FVector& StartTangent, const FVector& EndLocation, const FVector& EndTangent) {
	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);

	if (SplineMeshComponent) {
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->SetStaticMesh(RoadMesh);
		SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::X);
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshComponent->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true);
		SplineMeshComponent->RegisterComponent();

		SplineMeshComponent->SetRenderCustomDepth(true);
		SplineMeshComponent->CustomDepthStencilValue = 2;
	}

	RoadComponents.Add(SplineMeshComponent);
}

void ARoadActor::DestroyRoadMesh() {
	for (int32 i = 0; i < RoadComponents.Num(); ++i) {
		if (RoadComponents[i]) {
			RoadComponents[i]->DestroyComponent();
			RoadComponents[i] = nullptr;
		}
	}

	RoadComponents.Empty();
}

void ARoadActor::AddNewPoint(const FVector& Location) {
	if (!IsValid(SplineComponent) || !IsValid(RoadMesh)) return;

	SplineComponent->AddSplinePoint(Location, ESplineCoordinateSpace::World);
	if (SplineComponent->GetNumberOfSplinePoints() >= 2) {
		GenerateRoadMesh();
	}
}

bool ARoadActor::RemoveLastSplinePoint() {
	if (SplineComponent->GetNumberOfSplinePoints() != 0) {
		SplineComponent->RemoveSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 1);
		return true;
	}
	return false;
}

//void ARoadActor::UpdateRoad() {
//	if (SplinePoints.Num() < 2) {
//		DestroyRoadMesh();
//		return;
//	}
//
//	SplineComponent->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::World);
//
//	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
//	const FVector MeshBounds = RoadMesh->GetBounds().BoxExtent * 2.0f;
//	const float MeshWidth = MeshBounds.Y;
//	const float MeshLength = MeshBounds.X;
//	const float ScaleFactor = Width / MeshWidth;
//
//	if (RoadPointType == ERoadPointType::Sharp) {
//		for (int32 i = 0; i < NumPoints; ++i) {
//			SplineComponent->SetSplinePointType(i, ESplinePointType::CurveClamped, true);
//		}
//	}
//
//	int32 SegmentIndex = 0;
//
//	for (int32 i = 0; i < NumPoints - 1; ++i) {
//		const float StartDistance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i);
//		const float EndDistance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i + 1);
//		const float SegmentLength = EndDistance - StartDistance;
//		const int32 NumberOfSegments = FMath::CeilToInt(SegmentLength / MeshLength);
//
//		for (int32 j = 0; j < NumberOfSegments; ++j) {
//			if (SegmentIndex >= RoadComponents.Num()) {
//				USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
//				SplineMeshComponent->SetMobility(EComponentMobility::Movable);
//				SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
//				SplineMeshComponent->SetStaticMesh(RoadMesh);
//				SplineMeshComponent->RegisterComponent();
//
//				SplineMeshComponent->SetRenderCustomDepth(true);
//				SplineMeshComponent->CustomDepthStencilValue = 2;
//
//				RoadComponents.Add(SplineMeshComponent);
//			}
//
//			USplineMeshComponent* SplineMeshComponent = RoadComponents[SegmentIndex];
//
//			SplineMeshComponent->SetVisibility(true);
//
//			float StrechedMeshLength = SegmentLength / NumberOfSegments;
//
//			float LocalStartDistance = StartDistance + j * StrechedMeshLength - 200;
//			float LocalEndDistance = StartDistance + (j + 1) * StrechedMeshLength - 200;
//
//			const FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(LocalStartDistance, ESplineCoordinateSpace::Local);
//			const FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(LocalStartDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MeshLength);
//			const FVector EndLocation = SplineComponent->GetLocationAtDistanceAlongSpline(LocalEndDistance, ESplineCoordinateSpace::Local);
//			const FVector EndTangent = Spline->GetTangentAtDistanceAlongSpline(LocalEndDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MeshLength);
//
//			SplineMeshComponent->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true);
//			SplineMeshComponent->SetStartScale(FVector2D(ScaleFactor, 1));
//			SplineMeshComponent->SetEndScale(FVector2D(ScaleFactor, 1));
//			SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//			SplineMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
//			
//			// TODO:: Material
//
//			++SegmentIndex;
//		}
//	}
//
//	for (int32 i = SegmentIndex; i < RoadComponents.Num(); ++i) {
//		RoadComponents[i]->SetVisibility(false);
//	}
//}

void ARoadActor::SetState(ERoadActorState NewRoadActorState) {
	RoadActorState = NewRoadActorState;

	HandleStateChange();
}

ERoadActorState ARoadActor::GetState() const {
	return RoadActorState;
}

void ARoadActor::SetPointType(ERoadPointType NewRoadPointType) {
	RoadPointType = NewRoadPointType;
}

ERoadPointType ARoadActor::GetPointType() const {
	return RoadPointType;
}
