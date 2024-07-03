// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingCreationActor.h"
#include "GameFramework/Actor.h"
#include "WallActor.generated.h"

class ADoorActor;

UCLASS()
class ARCHVIZEXPLORER_API AWallActor : public ABuildingCreationActor {
	GENERATED_BODY()

public:
	friend class UWallPlacementMode;
	friend class USaveAndLoadMode;
	
	// Sets default values for this actor's properties
	AWallActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStartLocation(const FVector& NewStartLocation);
	const FVector& GetStartLocation() const;

	void SetEndLocation(const FVector& NewEndLocation);
	const FVector& GetEndLocation() const;

	void AttachDoorComponent(UPrimitiveComponent* ComponentToReplace, ADoorActor* DoorActor);
	void DetachDoorComponent(ADoorActor* DoorActor);
	void DestroyDoorComponents();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
	UStaticMesh* WallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
	UStaticMesh* DoorAttachableWallMesh;

	TArray<UStaticMeshComponent*> WallSegments;
	
	TMap<int32, ADoorActor*> IndexDoorMapping;

	double GetLength() const;
	void SetLength(double InValue);

private:
	FVector StartLocation;
	FVector EndLocation;

	double Length;

	void GenerateSegments(double Length = 0.0);
	void DestroySegments();
	void UpdateSegments();
	void HandlePreviewingState();
	void HandleMovingState();
	void HandleGeneratingState();
	void AdjustEdgeOffset();
	void UpdateWallLenghSlider();

	void UpdateWallProperties();

	void HandleMaterialChange(FMaterialAsset MaterialAsset);
	void ApplyMaterial();
};
