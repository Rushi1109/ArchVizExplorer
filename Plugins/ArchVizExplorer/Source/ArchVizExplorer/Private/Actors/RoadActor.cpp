// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RoadActor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARoadActor::ARoadActor() : RoadActorState{ERoadActorState::None}, RoadPointType{ERoadPointType::Sharp}, RoadMesh{ nullptr }, Width{200.f} {
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

	if (IsValid(MaterialWidgetRef)) {
		MaterialWidget = CreateWidget<UMaterialWidget>(GetWorld(), MaterialWidgetRef);

		if (IsValid(MaterialWidget->MaterialScrollBox)) {
			MaterialWidget->MaterialScrollBox->PopulateWidget(MaterialWidget->RoadMaterialDataAsset);
			MaterialWidget->MaterialScrollBox->OnItemSelected.BindUObject(this, &ARoadActor::HandleMaterialChange);
		}
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
	if (!IsValid(SplineComponent) || !IsValid(RoadMesh) || RoadActorState != ERoadActorState::Generating) return;

	if (!SplinePoints.IsEmpty()) {
		if (UKismetMathLibrary::Vector_Distance(Location, SplinePoints[SplinePoints.Num() - 1]) < 400) {
			return;
		}

	}

	SplinePoints.Add(Location);
	UpdateRoad();
}

bool ARoadActor::RemoveLastSplinePoint() {
	if(RoadActorState != ERoadActorState::Generating && RoadActorState != ERoadActorState::Selected)	return false;

	if (!SplinePoints.IsEmpty()) {
		SplinePoints.RemoveAt(SplinePoints.Num() - 1);
		UpdateRoad();
		return true;
	}
	return false;
}

void ARoadActor::UpdateRoad() {
	if (SplinePoints.Num() < 2) {
		DestroyRoadMesh();
		return;
	}

	UpdateSplinePoints();
	ConfigureSplinePointTypes();
	UpdateRoadMesh();
}

void ARoadActor::UpdateSplinePoints() {
	SplineComponent->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::World);
}

void ARoadActor::ConfigureSplinePointTypes() {
	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	if (RoadPointType == ERoadPointType::Sharp) {
		for (int32 i = 0; i < NumPoints; ++i) {
			SplineComponent->SetSplinePointType(i, ESplinePointType::CurveClamped, true);
		}
	}
}

void ARoadActor::UpdateRoadMesh() {
	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	const FVector MeshBounds = RoadMesh->GetBounds().BoxExtent * 2.0f;
	const float MeshWidth = MeshBounds.Y;
	const float MeshLength = MeshBounds.X;
	const float ScaleFactor = Width / MeshWidth;

	int32 SegmentIndex = 0;

	for (int32 i = 0; i < NumPoints - 1; ++i) {
		const float StartDistance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i);
		const float EndDistance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i + 1);
		const float SegmentLength = EndDistance - StartDistance;
		const int32 NumberOfSegments = FMath::CeilToInt(SegmentLength / MeshLength);

		for (int32 j = 0; j < NumberOfSegments; ++j) {
			UpdateOrCreateSegment(SegmentIndex, StartDistance, SegmentLength, MeshLength, ScaleFactor, j, NumberOfSegments);
			++SegmentIndex;
		}
	}

	HideUnusedSegments(SegmentIndex);
}

void ARoadActor::UpdateOrCreateSegment(int32 SegmentIndex, float StartDistance, float SegmentLength, float MeshLength, float ScaleFactor, int32 SegmentPartIndex, int32 NumberOfSegments) {
	if (SegmentIndex >= RoadComponents.Num()) {
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshComponent->SetStaticMesh(RoadMesh);
		SplineMeshComponent->RegisterComponent();

		SplineMeshComponent->SetRenderCustomDepth(true);
		SplineMeshComponent->CustomDepthStencilValue = 2;

		RoadComponents.Add(SplineMeshComponent);
	}

	USplineMeshComponent* SplineMeshComponent = RoadComponents[SegmentIndex];
	SplineMeshComponent->SetVisibility(true);

	float StretchedMeshLength = SegmentLength / NumberOfSegments;
	float LocalStartDistance = StartDistance + SegmentPartIndex * StretchedMeshLength - 200;
	float LocalEndDistance = StartDistance + (SegmentPartIndex + 1) * StretchedMeshLength - 200;

	const FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(LocalStartDistance, ESplineCoordinateSpace::Local);
	const FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(LocalStartDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MeshLength);
	const FVector EndLocation = SplineComponent->GetLocationAtDistanceAlongSpline(LocalEndDistance, ESplineCoordinateSpace::Local);
	const FVector EndTangent = SplineComponent->GetTangentAtDistanceAlongSpline(LocalEndDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MeshLength);

	SplineMeshComponent->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true);
	SplineMeshComponent->SetStartScale(FVector2D(ScaleFactor, 1));
	SplineMeshComponent->SetEndScale(FVector2D(ScaleFactor, 1));
	SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SplineMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

	if (IsValid(Material)) {
		SplineMeshComponent->SetMaterial(0, Material);
	}
}

void ARoadActor::HideUnusedSegments(int32 StartIndex) {
	for (int32 i = StartIndex; i < RoadComponents.Num(); ++i) {
		RoadComponents[i]->SetVisibility(false);
	}
}

void ARoadActor::SetState(ERoadActorState NewRoadActorState) {
	RoadActorState = NewRoadActorState;

	HandleStateChange();
}

ERoadActorState ARoadActor::GetState() const {
	return RoadActorState;
}

void ARoadActor::SetPointType(ERoadPointType NewRoadPointType) {
	RoadPointType = NewRoadPointType;

	UpdateRoad();
}

ERoadPointType ARoadActor::GetPointType() const {
	return RoadPointType;
}

TArray<FVector> ARoadActor::GetSplinePoints() const {
	return SplinePoints;
}

void ARoadActor::SetSplinePoints(TArray<FVector> InSplinePoints) {
	SplinePoints = InSplinePoints;
}

float ARoadActor::GetWidth() const {
	return Width;
}

void ARoadActor::SetWidth(float InWidth) {
	Width = InWidth;
}

void ARoadActor::HandleMaterialChange(FMaterialAsset MaterialAsset) {
	if (IsValid(MaterialAsset.Material)) {
		Material = MaterialAsset.Material;
		UpdateRoad();
	}
}
