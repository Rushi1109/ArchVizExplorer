// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/BuildingCreationActor.h"
#include "UMG/Public/Blueprint/UserWidget.h"

EBuildingActorState ABuildingCreationActor::GetState() {
	return State;
}

void ABuildingCreationActor::SetState(EBuildingActorState NewState) {
	State = NewState;
}

void ABuildingCreationActor::ShowPropertyPanel() {
	if(IsValid(PropertyPanel)) {
		PropertyPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABuildingCreationActor::HidePropertyPanel() {
	if(IsValid(PropertyPanel)) {
		PropertyPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABuildingCreationActor::RotateActor(double Degrees) {
	FRotator CurrentRotation = GetActorRotation();

	CurrentRotation.Yaw = static_cast<int>(CurrentRotation.Yaw + Degrees) % 360;

	SetActorRotation(CurrentRotation);
}
