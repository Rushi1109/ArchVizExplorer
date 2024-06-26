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

	ProceduralMeshComponent->ContainsPhysicsTriMeshData(true);
	ProceduralMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProceduralMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	ProceduralMeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
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

void AFloorActor::GenerateFloor(const FVector& Dimensions, const FVector& Offset) {
	DestroyFloor();

	ProceduralMeshGenerator::GenerateCube(ProceduralMeshComponent, 0, Dimensions, Offset);
}

void AFloorActor::DestroyFloor() {
	ProceduralMeshComponent->ClearAllMeshSections();
}

void AFloorActor::HandlePreviewingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::SnapToGridFloored(HitResult.Location);

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

	FVector Dimensions{ FMath::Abs(XFloorLength) + (2 * EdgeOffset), FMath::Abs(YFloorLength) + (2 * EdgeOffset), 2 };
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

	FVector NewStartLocation{ StartLocation };

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

	GenerateFloor(Dimensions, Offset);
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