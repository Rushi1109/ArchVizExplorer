// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteriorActor.h"
#include "Actors/BuildingCreation/WallActor.h"

// Sets default values
AInteriorActor::AInteriorActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteriorStaticMesh"));
	StaticMeshComponent->SetupAttachment(SceneRoot);
}

void AInteriorActor::HandleStateChange() {
	if (InteriorActorState == EInteriorActorState::Selected) {
		ShowPropertyPanel();
		HighlightSelectedActor();
	}
	else if (InteriorActorState == EInteriorActorState::Moving) {
		HighlightSelectedActor();
	}
	else {
		HidePropertyPanel();
		UnHighlightDeselectedActor();
	}
}

void AInteriorActor::SetState(EInteriorActorState NewInteriorActorState) {
	InteriorActorState = NewInteriorActorState;

	HandleStateChange();
}

EInteriorActorState AInteriorActor::GetState() const {
	return InteriorActorState;
}

void AInteriorActor::SetAssetType(EInteriorAssetType NewInteriorAssetType) {
	InteriorAssetType = NewInteriorAssetType;
}

EInteriorAssetType AInteriorActor::GetAssetType() const {
	return InteriorAssetType;
}


void AInteriorActor::SetStaticMesh(UStaticMesh* NewStaticMesh) {
	StaticMeshComponent->SetStaticMesh(NewStaticMesh);
}

UStaticMesh* AInteriorActor::GetStaticMesh() const {
	return StaticMeshComponent->GetStaticMesh();
}

// Called when the game starts or when spawned
void AInteriorActor::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(PropertyPanelRef)) {
		PropertyPanel = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelRef);
		PropertyPanel->WidgetSwitcher->SetActiveWidgetIndex(4);
	}
}

// Called every frame
void AInteriorActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (InteriorActorState == EInteriorActorState::Previewing) {
		HandlePreviewingState();
	}
	else if (InteriorActorState == EInteriorActorState::Moving) {
		HandleMovingState();
	}
}

void AInteriorActor::HandlePreviewingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});

	if (IsValid(HitResult.GetActor())) {
		SetActorLocation(HitResult.Location);

		if (HitResult.GetActor()->IsA(AWallActor::StaticClass()) && InteriorAssetType == EInteriorAssetType::WallPlaceable) {
			SetActorRotation(HitResult.GetActor()->GetActorRotation());
		}
	}
}

void AInteriorActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});

	if (IsValid(HitResult.GetActor())) {
		SetActorLocation(HitResult.Location);

		if (HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
			SetActorRotation(HitResult.GetActor()->GetActorRotation());
		}
	}
}