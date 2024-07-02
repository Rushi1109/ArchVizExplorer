// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BuildingCreation/BuildingCreationActor.h"
#include "DoorActor.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API ADoorActor : public ABuildingCreationActor {
	GENERATED_BODY()

public:
	friend class UDoorPlacementMode;

	// Sets default values for this actor's properties
	ADoorActor();

	bool bIsOpen;
	int32 ParentWallComponentIndex = -1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door")
	UStaticMesh* DoorFrameMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door")
	UStaticMesh* DoorMesh;

private:
	UPROPERTY()
	UStaticMeshComponent* DoorFrameComponent;

	UPROPERTY()
	UStaticMeshComponent* DoorComponent;

	void HandlePreviewState();
	void HandleMovingState();
};
