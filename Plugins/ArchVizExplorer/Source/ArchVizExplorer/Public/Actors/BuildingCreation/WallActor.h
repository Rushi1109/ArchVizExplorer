// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingCreationActor.h"
#include "GameFramework/Actor.h"
#include "WallActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AWallActor : public ABuildingCreationActor {
	GENERATED_BODY()

public:
	friend class UWallPlacementMode;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
	UStaticMesh* WallMesh;

	UPROPERTY(VisibleAnywhere, Category = "Wall")
	TArray<UStaticMeshComponent*> WallSegments;

private:
	FVector StartLocation;
	FVector EndLocation;

	void GenerateSegments(double Length = 0.0);
	void DestroySegments();
	void HandlePreviewingState();
	void HandleMovingState();
	void HandleGeneratingState();
	void AdjustEdgeOffset();
};
