// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/DoorActor.h"
#include "Actors/BuildingCreation/WallActor.h"
#include "Widgets/PropertyPanelWidget.h"

ADoorActor::ADoorActor() : bIsOpen{false}, DoorFrameMesh{nullptr}, DoorMesh{nullptr} {
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame Component"));
	DoorFrameComponent->SetupAttachment(SceneComponent);

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Component"));
	DoorComponent->SetupAttachment(DoorFrameComponent, TEXT("Door"));
}

void ADoorActor::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(DoorFrameMesh)) {
		DoorFrameComponent->SetStaticMesh(DoorFrameMesh);
	}
	if (IsValid(DoorMesh)) {
		DoorComponent->SetStaticMesh(DoorMesh);
	}

	if (IsValid(PropertyPanelRef)) {
		PropertyPanel = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelRef);
		PropertyPanel->WidgetSwitcher->SetActiveWidgetIndex(1);
	}
}

void ADoorActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (State == EBuildingActorState::Previewing) {
		HandlePreviewState();
	}
	else if (State == EBuildingActorState::Moving) {
		HandleMovingState();
	}
}

void ADoorActor::HandlePreviewState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	//HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	if (HitResult.GetActor() && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		SetActorRotation(HitResult.GetActor()->GetActorRotation());
	}

	SetActorLocation(HitResult.Location);
}

void ADoorActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	//HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	if (HitResult.GetActor() && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		SetActorRotation(HitResult.GetActor()->GetActorRotation());
	}

	SetActorLocation(HitResult.Location);
}