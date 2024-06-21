// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingCreationActor.h"
#include "GameFramework/Actor.h"
#include "WallActor.generated.h"

class UProceduralMeshComponent;

UCLASS()
class ARCHVIZEXPLORER_API AWallActor : public ABuildingCreationActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWallActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Wall")
	//UProceduralMeshComponent* ProceduralWallGenerator;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline int32 GetSegmentIndex() const {
		return SegmentIndex;
	}
	inline void SetSegmentIndex(int32 Index) {
		SegmentIndex = Index;
	}

	inline FRotator GetSegmentRotation() const {
		return SegmentRotation;
	}
	inline void SetSegmentRotation(const FRotator& Rotation) {
		SegmentRotation = Rotation;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
	UStaticMesh* WallMesh;

	UPROPERTY(VisibleAnywhere, Category = "Wall")
	TArray<UStaticMeshComponent*> WallSegments;

	UPROPERTY(VisibleDefaultsOnly, Category = "Wall")
	UStaticMeshComponent* PreviewWallSegment;

	UFUNCTION()
	void DestroyPreviewWallSegment();

	FHitResult GetHitResult() const;
private:
	int32 SegmentIndex;

	FRotator SegmentRotation;
};
