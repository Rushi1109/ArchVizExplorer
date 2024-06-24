// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/FloorActor.h"
#include "ProceduralMeshComponent.h"
#include "Utilities/ArchVizUtility.h"
#include "ProceduralMeshGenerator.h"

// Sets default values
AFloorActor::AFloorActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh Component"));
	ProceduralMeshComponent->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AFloorActor::BeginPlay() {
	Super::BeginPlay();

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

void AFloorActor::GenerateFloor(const FVector& Dimensions) {
	DestroyFloor();

	ProceduralMeshGenerator::GenerateCube(ProceduralMeshComponent, 0, Dimensions, Dimensions / 2);
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

	SetEndLocation(HitResult.Location);

	double XFloorLength = EndLocation.X - StartLocation.X;
	double YFloorLength = EndLocation.Y - StartLocation.Y;

	GenerateFloor(FVector{XFloorLength, YFloorLength, 10});
}