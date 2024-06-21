// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/BuildingCreationActor.h"

void ABuildingCreationActor::RotateActorBy90Degrees() {
	FRotator CurrentRotation = GetActorRotation();

	CurrentRotation.Yaw = static_cast<int>(CurrentRotation.Yaw + 90) % 360;

	SetActorRotation(FRotator{CurrentRotation});
}
