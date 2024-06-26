// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/RoofActor.h"
#include "ProceduralMeshComponent.h"
#include "Utilities/ArchVizUtility.h"
#include "ProceduralMeshGenerator.h"

// Sets default values
ARoofActor::ARoofActor() {
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

void ARoofActor::GenerateRoof(const FVector& Dimensions, const FVector& Offset) {
	DestroyRoof();

	ProceduralMeshGenerator::GenerateCube(ProceduralMeshComponent, 0, Dimensions, Offset);
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

	FVector Dimensions{ FMath::Abs(XFloorLength) + (2 * EdgeOffset), FMath::Abs(YFloorLength) + (2 * EdgeOffset), 20 };
	FVector Offset{ Dimensions / 2 };

	if (XFloorLength >= 0.0 && YFloorLength >= 0.0) {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 0.0 });
	}
	else if (XFloorLength >= 0.0 && YFloorLength < 0.0) {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 0.0,0.0,180.0 });
		Offset.Z *= -1.0;
	}
	else if (XFloorLength < 0.0 && YFloorLength >= 0.0) {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0,0.0 });
		Offset.Z *= -1.0;
	}
	else {
		ProceduralMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0, 180.0 });
	}

	FVector NewStartLocation{StartLocation};

	if (XFloorLength >= 0.0) {
		NewStartLocation.X -= EdgeOffset;
	}
	else {
		NewStartLocation.X += EdgeOffset;
	}
	if (YFloorLength >= 0.0) {
		NewStartLocation.Y -= EdgeOffset;
	}
	else {
		NewStartLocation.Y += EdgeOffset;
	}

	SetActorLocation(NewStartLocation);

	GenerateRoof(Dimensions, Offset);
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