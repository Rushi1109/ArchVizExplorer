// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/RoofActor.h"
#include "ProceduralMeshComponent.h"
#include "Utilities/ArchVizUtility.h"
#include "ProceduralMeshGenerator.h"
#include "Widgets/PropertyPanelWidget.h"

// Sets default values
ARoofActor::ARoofActor() : Dimensions{100, 100, 10}, Offset{50, 50, 5} {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh Component"));
	ProceduralMeshComponent->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void ARoofActor::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(PropertyPanelRef)) {
		PropertyPanel = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelRef);
		PropertyPanel->WidgetSwitcher->SetActiveWidgetIndex(3);
	}

	if (IsValid(MaterialWidgetRef)) {
		MaterialWidget = CreateWidget<UMaterialWidget>(GetWorld(), MaterialWidgetRef);

		if (IsValid(MaterialWidget->MaterialScrollBox)) {
			MaterialWidget->MaterialScrollBox->PopulateWidget(MaterialWidget->RoofMaterialDataAsset);
			MaterialWidget->MaterialScrollBox->OnItemSelected.BindUObject(this, &ARoofActor::HandleMaterialChange);
		}
	}
}

// Called every frame
void ARoofActor::Tick(float DeltaTime) {
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

void ARoofActor::GenerateRoof() {
	DestroyRoof();

	FVector RoofOffset;

	AdjustDirection(RoofOffset);

	ProceduralMeshGenerator::GenerateCube(ProceduralMeshComponent, 0, Dimensions, RoofOffset);

	ApplyPreviewMaterial();
}

void ARoofActor::DestroyRoof() {
	ProceduralMeshComponent->ClearAllMeshSections();
}

void ARoofActor::HandlePreviewingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::SnapToGridFloored(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void ARoofActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void ARoofActor::HandleGeneratingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	if (EndLocation == HitResult.Location) {
		return;
	}
	SetEndLocation(HitResult.Location);

	double XFloorLength = EndLocation.X - StartLocation.X;
	double YFloorLength = EndLocation.Y - StartLocation.Y;

	double EdgeOffset{10.0};

	Dimensions = { FMath::Abs(XFloorLength) + (2 * EdgeOffset), FMath::Abs(YFloorLength) + (2 * EdgeOffset), 20 };
	Offset = { FMath::Abs(XFloorLength) / 2, FMath::Abs(YFloorLength) / 2, 10 };

	GenerateRoof();
}

void ARoofActor::AdjustDirection(FVector& RoofOffset) {
	double XFloorLength = EndLocation.X - StartLocation.X;
	double YFloorLength = EndLocation.Y - StartLocation.Y;

	double EdgeOffset{ 10.0 };

	RoofOffset = Offset;

	if (XFloorLength >= 0.0 && YFloorLength >= 0.0) {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 0.0 });
	}
	else if (XFloorLength >= 0.0 && YFloorLength < 0.0) {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 0.0,0.0,180.0 });
		RoofOffset.Z *= -1.0;
	}
	else if (XFloorLength < 0.0 && YFloorLength >= 0.0) {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0,0.0 });
		RoofOffset.Z *= -1.0;
	}
	else {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0, 180.0 });
	}
}

void ARoofActor::HandleMaterialChange(FMaterialAsset MaterialAsset) {
	if (IsValid(MaterialAsset.Material)) {
		Material = MaterialAsset.Material;
		ApplyMaterial();
	}
}

void ARoofActor::ApplyMaterial() {
	if (IsValid(Material)) {
		if (auto* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this)) {
			DynamicMaterial->SetVectorParameterValue(FName{"Tiling/Offset"}, FLinearColor(Dimensions.Y / 400.0, Dimensions.X / 400.0, 0.0, 0.0));

			ProceduralMeshComponent->SetMaterial(0, DynamicMaterial);
		}
	}
}

void ARoofActor::UpdateRoofDimensionSlider() {
	if (IsValid(PropertyPanel)) {
		PropertyPanel->RoofLengthSpinBox->SetValue(FMath::Abs(EndLocation.X - StartLocation.X));
		PropertyPanel->RoofWidthSpinBox->SetValue(FMath::Abs(EndLocation.Y - StartLocation.Y));
		PropertyPanel->RoofHeightSpinBox->SetValue(FMath::Abs(20));
	}
}

void ARoofActor::UpdateRoofProperties() {
	if (IsValid(PropertyPanel)) {
		PropertyPanel->RoofLengthSpinBox->SetValue(FMath::Abs(Dimensions.X));
		PropertyPanel->RoofWidthSpinBox->SetValue(FMath::Abs(Dimensions.Y));
		PropertyPanel->RoofHeightSpinBox->SetValue(FMath::Abs(Dimensions.Z));
	}
}

void ARoofActor::SetStartLocation(const FVector& NewStartLocation) {
	StartLocation = NewStartLocation;
}

const FVector& ARoofActor::GetStartLocation() {
	return StartLocation;
}

void ARoofActor::SetEndLocation(const FVector& NewEndLocation) {
	EndLocation = NewEndLocation;
}

const FVector& ARoofActor::GetEndLocation() {
	return EndLocation;
}

void ARoofActor::SetDimensions(const FVector& InDImensions) {
	Dimensions = InDImensions;
}

const FVector& ARoofActor::GetDimensions() const {
	return Dimensions;
}

void ARoofActor::SetOffset(const FVector& InOffset) {
	Offset = InOffset;
}

const FVector& ARoofActor::GetOffset() const {
	return Offset;
}

