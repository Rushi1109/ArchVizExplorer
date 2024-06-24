// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationModes/BuildingCreationSubModes.h"

void UBuildingCreationSubMode::InitParams(APlayerController* Controller) {
	PlayerController = Controller;
}

FHitResult UBuildingCreationSubMode::GetHitResult(const TArray<AActor*>& ActorsToIgnore) const {
	FHitResult HitResult{};

	FVector WorldLocation{}, WorldDirection{};

	if (IsValid(PlayerController) && PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.AddIgnoredActor(PlayerController->GetPawn());

		for (const auto& Actor : ActorsToIgnore) {
			CollisionQueryParams.AddIgnoredActor(Actor);
		}

		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
	}

	return HitResult;
}