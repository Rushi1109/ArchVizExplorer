// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/FloorActor.h"
#include "ProceduralMeshComponent.h"

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

}

