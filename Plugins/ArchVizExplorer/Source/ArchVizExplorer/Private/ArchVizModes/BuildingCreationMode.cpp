// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingCreationMode.h"

UBuildingCreationMode::UBuildingCreationMode() : PlayerController{ nullptr }, InputMappingContext{nullptr} {

}

void UBuildingCreationMode::InitParam(APlayerController* Controller) {
	PlayerController = Controller;
}

void UBuildingCreationMode::SetBuildingModeEntity(EBuildingModeEntity Entity) {
	BuildingModeEntity = Entity;
}

void UBuildingCreationMode::SetupInputMapping() {

}

void UBuildingCreationMode::EnterMode() {

}

void UBuildingCreationMode::ExitMode() {

}