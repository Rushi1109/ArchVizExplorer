// Fill out your copyright notice in the Description page of Project Settings.


#include "WallActor.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AWallActor::AWallActor() : SegmentIndex{ -1 }, SegmentRotation{0.0} {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	PreviewWallSegment = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewWallSegment"));
	PreviewWallSegment->SetupAttachment(SceneRoot);

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

