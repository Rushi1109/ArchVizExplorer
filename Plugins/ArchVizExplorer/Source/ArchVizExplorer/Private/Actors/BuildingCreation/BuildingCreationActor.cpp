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
	else {
		HidePropertyPanel();
		UnHighlightDeselectedActor();
	}
}

void ABuildingCreationActor::HighlightSelectedActor() {
	TSet<UActorComponent*> ActorComponents = GetComponents();

	for (auto& ActorComponent : ActorComponents) {
		if (UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(ActorComponent)) {
			Component->SetRenderCustomDepth(true);
			Component->CustomDepthStencilValue = 2;
		}
	}
}

void ABuildingCreationActor::UnHighlightDeselectedActor() {
	TSet<UActorComponent*> ActorComponents = GetComponents();

	for (auto& ActorComponent : ActorComponents) {
		if (UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(ActorComponent)) {
			Component->SetRenderCustomDepth(false);
		}
	}
}

void ABuildingCreationActor::ShowPropertyPanel() {
	if(IsValid(PropertyPanel)) {
		PropertyPanel->AddToViewport();
	}
}

void ABuildingCreationActor::HidePropertyPanel() {
	if(IsValid(PropertyPanel)) {
		PropertyPanel->RemoveFromParent();
	}
}

void ABuildingCreationActor::RotateActor(double Degrees) {
	FRotator CurrentRotation = GetActorRotation();

	CurrentRotation.Yaw = static_cast<int>(CurrentRotation.Yaw + Degrees) % 360;

	SetActorRotation(CurrentRotation);
}
