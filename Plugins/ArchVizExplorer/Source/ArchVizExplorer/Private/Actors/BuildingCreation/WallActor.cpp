// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/WallActor.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AWallActor::AWallActor() : SegmentIndex{ -1 }, SegmentRotation{0.0}, PreviewWallSegment{ nullptr } {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	//ProceduralWallGenerator = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralWallGenerator"));
	//ProceduralWallGenerator->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AWallActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AWallActor::DestroyPreviewWallSegment() {
	if(IsValid(PreviewWallSegment)){ 
		PreviewWallSegment->DestroyComponent();
	}
	PreviewWallSegment = nullptr;
}

FHitResult AWallActor::GetHitResult() const {
	FHitResult MouseHitResult{};

	FVector WorldLocation{}, WorldDirection{};

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.AddIgnoredActor(this);
		CollisionQueryParams.AddIgnoredActor(PlayerController->GetPawn());

		GetWorld()->LineTraceSingleByChannel(MouseHitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);

		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.0f);
		if (MouseHitResult.bBlockingHit) {
			DrawDebugPoint(GetWorld(), MouseHitResult.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);
		}
	}

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, MouseHitResult.ImpactPoint.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, MouseHitResult.Location.ToString());
	}

	return MouseHitResult;
}