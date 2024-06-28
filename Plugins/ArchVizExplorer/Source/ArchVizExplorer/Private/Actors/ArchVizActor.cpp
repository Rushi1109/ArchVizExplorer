// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ArchVizActor.h"

// Sets default values
AArchVizActor::AArchVizActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArchVizActor::BeginPlay() {
	Super::BeginPlay();

}

void AArchVizActor::DestroyActor() {
	Destroy();
}

void AArchVizActor::HighlightSelectedActor() {
	TSet<UActorComponent*> ActorComponents = GetComponents();

	for (auto& ActorComponent : ActorComponents) {
		if (UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(ActorComponent)) {
			Component->SetRenderCustomDepth(true);
			Component->CustomDepthStencilValue = 2;
		}
	}
}

void AArchVizActor::UnHighlightDeselectedActor() {
	TSet<UActorComponent*> ActorComponents = GetComponents();

	for (auto& ActorComponent : ActorComponents) {
		if (UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(ActorComponent)) {
			Component->SetRenderCustomDepth(false);
		}
	}
}

void AArchVizActor::ShowPropertyPanel() {
	if (IsValid(PropertyPanel)) {
		PropertyPanel->AddToViewport();
	}
}

void AArchVizActor::HidePropertyPanel() {
	if (IsValid(PropertyPanel)) {
		PropertyPanel->RemoveFromParent();
	}
}

void AArchVizActor::RotateActor(double Degrees) {
	FRotator CurrentRotation = GetActorRotation();

	CurrentRotation.Yaw = static_cast<int>(CurrentRotation.Yaw + Degrees) % 360;

	SetActorRotation(CurrentRotation);
}


FHitResult AArchVizActor::GetHitResult(const TArray<AActor*>& ActorsToIgnore) const {
	FHitResult HitResult{};

	FVector WorldLocation{}, WorldDirection{};
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.AddIgnoredActor(PlayerController->GetPawn());
		CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
	}

	return HitResult;
}

// Called every frame
void AArchVizActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

