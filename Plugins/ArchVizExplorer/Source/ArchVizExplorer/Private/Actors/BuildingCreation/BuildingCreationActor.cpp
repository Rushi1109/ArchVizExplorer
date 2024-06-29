// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildingCreation/BuildingCreationActor.h"
#include "Widgets/PropertyPanelWidget.h"
#include "Components/PrimitiveComponent.h"

ABuildingCreationActor::ABuildingCreationActor() : State{ EBuildingActorState::None } {
	PrimaryActorTick.bCanEverTick = true;
}

EBuildingActorState ABuildingCreationActor::GetState() {
	return State;
}

void ABuildingCreationActor::SetState(EBuildingActorState NewState) {
	State = NewState;

	HandleStateChange();
}

void ABuildingCreationActor::HandleStateChange() {
	if (State == EBuildingActorState::Selected) {
		ShowPropertyPanel();
		HighlightSelectedActor();
	}
	else if (State == EBuildingActorState::Moving) {
		HighlightSelectedActor();
	}
	else {
		HidePropertyPanel();
		UnHighlightDeselectedActor();
	}
}