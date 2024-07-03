// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/FloorActor.h"
#include "ProceduralMeshComponent.h"
#include "Utilities/ArchVizUtility.h"
#include "ProceduralMeshGenerator.h"
#include "Widgets/PropertyPanelWidget.h"

// Sets default values
AFloorActor::AFloorActor() : Dimensions{100, 100, 2}, Offset{50, 50,1} {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh Component"));
	ProceduralMeshComponent->SetupAttachment(SceneRoot);

	ProceduralMeshComponent->ContainsPhysicsTriMeshData(true);
	ProceduralMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProceduralMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	ProceduralMeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void AFloorActor::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(PropertyPanelRef)) {
		PropertyPanel = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelRef);
		PropertyPanel->WidgetSwitcher->SetActiveWidgetIndex(2);
	}


	if (IsValid(MaterialWidgetRef)) {
		MaterialWidget = CreateWidget<UMaterialWidget>(GetWorld(), MaterialWidgetRef);

		if (IsValid(MaterialWidget->MaterialScrollBox)) {
			MaterialWidget->MaterialScrollBox->PopulateWidget(MaterialWidget->FloorMaterialDataAsset);
			MaterialWidget->MaterialScrollBox->OnItemSelected.BindUObject(this, &AFloorActor::HandleMaterialChange);
		}
	}
}

// Called every frame
void AFloorActor::Tick(float DeltaTime) {
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

void AFloorActor::GenerateFloor() {
	DestroyFloor();

	FVector FloorDimension;
	FVector FloorOffset;

	AdjustDirection(FloorDimension, FloorOffset);

	ProceduralMeshGenerator::GenerateCube(ProceduralMeshComponent, 0, FloorDimension, FloorOffset);

	ApplyMaterial();
}

void AFloorActor::DestroyFloor() {
	ProceduralMeshComponent->ClearAllMeshSections();
}

void AFloorActor::HandlePreviewingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void AFloorActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void AFloorActor::HandleGeneratingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::SnapToGrid(HitResult.Location);

	if (EndLocation == HitResult.Location) {
		return;
	}
	SetEndLocation(HitResult.Location);

	double XFloorLength = EndLocation.X - StartLocation.X;
	double YFloorLength = EndLocation.Y - StartLocation.Y;

	double EdgeOffset{ 10.0 };

	Dimensions = { FMath::Abs(XFloorLength) + (2 * EdgeOffset), FMath::Abs(YFloorLength) + (2 * EdgeOffset), 2 };
	Offset = { FMath::Abs(XFloorLength) / 2, FMath::Abs(YFloorLength) / 2, 1 };

	GenerateFloor();
}

void AFloorActor::AdjustDirection(FVector& FloorDimensions, FVector& FloorOffset) {
	double XFloorLength = EndLocation.X - StartLocation.X;
	double YFloorLength = EndLocation.Y - StartLocation.Y;

	FloorDimensions = Dimensions;
	FloorOffset = Offset;

	double EdgeOffset{ 10.0 };

	if (XFloorLength >= 0.0 && YFloorLength >= 0.0) {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 0.0 });
	}
	else if (XFloorLength >= 0.0 && YFloorLength < 0.0) {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 0.0,0.0,180.0 });
		FloorOffset.Z *= -1.0;
	}
	else if (XFloorLength < 0.0 && YFloorLength >= 0.0) {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0,0.0 });
		FloorOffset.Z *= -1.0;
	}
	else {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0, 180.0 });
	}
}

void AFloorActor::UpdateFloorDimensionSlider() {
	if (IsValid(PropertyPanel)) {
		PropertyPanel->FloorLengthSpinBox->SetValue(FMath::Abs(EndLocation.X - StartLocation.X));
		PropertyPanel->FloorWidthSpinBox->SetValue(FMath::Abs(EndLocation.Y - StartLocation.Y));
		PropertyPanel->FloorHeightSpinBox->SetValue(FMath::Abs(2));
	}
}

void AFloorActor::UpdateFloorProperties() {
	if (IsValid(PropertyPanel)) {
		PropertyPanel->FloorLengthSpinBox->SetValue(FMath::Abs(Dimensions.X));
		PropertyPanel->FloorWidthSpinBox->SetValue(FMath::Abs(Dimensions.Y));
		PropertyPanel->FloorHeightSpinBox->SetValue(FMath::Abs(Dimensions.Z));
	}
}

void AFloorActor::HandleMaterialChange(FMaterialAsset MaterialAsset) {
	if (IsValid(MaterialAsset.Material)) {
		Material = MaterialAsset.Material;
		ApplyMaterial();
	}
}

void AFloorActor::ApplyMaterial() {
	if (IsValid(Material)) {
		if (auto* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this)) {
			DynamicMaterial->SetVectorParameterValue(FName{ "Tiling/Offset" }, FLinearColor( Dimensions.Y / 400.0, Dimensions.X / 400.0, 0.0, 0.0 ));

			ProceduralMeshComponent->SetMaterial(0, DynamicMaterial);
		}
	}
}

void AFloorActor::SetStartLocation(const FVector& NewStartLocation) {
	StartLocation = NewStartLocation;
}

const FVector& AFloorActor::GetStartLocation() {
	return StartLocation;
}

void AFloorActor::SetEndLocation(const FVector& NewEndLocation) {
	EndLocation = NewEndLocation;
}

const FVector& AFloorActor::GetEndLocation() {
	return EndLocation;
}

void AFloorActor::SetDimensions(const FVector& InDImensions) {
	Dimensions = InDImensions;
}

const FVector& AFloorActor::GetDimensions() const {
	return Dimensions;
}

void AFloorActor::SetOffset(const FVector& InOffset) {
	Offset = InOffset;
}

const FVector& AFloorActor::GetOffset() const {
	return Offset;
}

