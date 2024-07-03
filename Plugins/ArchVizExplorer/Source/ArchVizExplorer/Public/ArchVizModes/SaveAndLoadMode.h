// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizModes/ArchVizMode.h"
#include "SaveAndLoadMode.generated.h"

class ARoadActor;
class AWallActor;
class ADoorActor;
class ARoofActor;
class AFloorActor;
class AInteriorActor;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORER_API USaveAndLoadMode : public UArchVizMode {
	GENERATED_BODY()

public:
	USaveAndLoadMode();

	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;

protected:
	UPROPERTY()
	TArray<FString> SlotsList;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoadActor> RoadActorRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWallActor> WallActorRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFloorActor> FloorActorRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoofActor> RoofActorRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADoorActor> DoorActorRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AInteriorActor> InteriorActorRef;

private:
	FString CurrentSlotName;

	UFUNCTION()
	void HandleNewProjectMenuButtonClick();

	UFUNCTION()
	void HandleSaveProjectMenuButtonClick();

	UFUNCTION()
	void HandleLoadProjectMenuButtonClick();

	UFUNCTION()
	void HandleSaveButtonClick();

	UFUNCTION()
	void HandleSlotLoadClick(const FString& SlotName);
	
	UFUNCTION()
	void HandleSlotDeleteClick(const FString& SlotName);

	void DeleteSlotData(const FString& SlotName);

	TArray<FString> GetSaveSlots();

	void SaveGame(const FString& SlotName);
	void LoadGame(const FString& SlotName);

	void UpdateWidgetWithNewSlots();
	void ClearWholeWorld();
};
