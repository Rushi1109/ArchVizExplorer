// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ArchVizActor.h"
#include "InteriorActor.generated.h"

UENUM(BlueprintType)
enum class EInteriorActorState : uint8 {
	None,
	Previewing,
	Selected,
	Moving
};

UENUM(BlueprintType)
enum class EInteriorAssetType : uint8 {
	FloorPlaceable,
	WallPlaceable,
	RoofPlaceable
};

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API AInteriorActor : public AArchVizActor {
	GENERATED_BODY()

public:
	friend class UInteriorDesignMode;

	// Sets default values for this actor's properties
	AInteriorActor();

	void HandleStateChange();

	void SetState(EInteriorActorState NewInteriorActorState);
	EInteriorActorState GetState() const;

	void SetAssetType(EInteriorAssetType NewInteriorAssetType);
	EInteriorAssetType GetAssetType() const;

	void SetStaticMesh(UStaticMesh* NewStaticMesh);
	UStaticMesh* GetStaticMesh() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void HandlePreviewingState();
	void HandleMovingState();

	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY()
	UStaticMesh* StaticMesh;

	EInteriorActorState InteriorActorState;
	EInteriorAssetType InteriorAssetType;
};
