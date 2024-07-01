// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizActor.h"
#include "GameFramework/Actor.h"
#include "RoadActor.generated.h"

class USplineMeshComponent;
class USplineComponent;

UENUM()
enum class ERoadActorState : uint8 {
	None,
	Generating,
	Selected
};

UENUM()
enum class ERoadPointType : uint8 {
	Sharp,
	Curved
};

UCLASS()
class ARCHVIZEXPLORER_API ARoadActor : public AArchVizActor {
	GENERATED_BODY()

public:
	friend class URoadConstructionMode;

	// Sets default values for this actor's properties
	ARoadActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Road")
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Road")
	TArray<USplineMeshComponent*> RoadComponents;

	UPROPERTY(VisibleDefaultsOnly, Category = "Road")
	TArray<FVector> SplinePoints;

	UPROPERTY(EditDefaultsOnly, Category = "Road")
    UStaticMesh* RoadMesh;

	float Width;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void HandleStateChange();

	UFUNCTION(BlueprintCallable)
    void DestroyRoadMesh();

	void AddNewPoint(const FVector& Location);
	bool RemoveLastSplinePoint();

	void UpdateRoad();
	void UpdateSplinePoints();
	void ConfigureSplinePointTypes();
	void UpdateRoadMesh();
	void UpdateOrCreateSegment(int32 SegmentIndex, float StartDistance, float SegmentLength, float MeshLength, float ScaleFactor, int32 SegmentPartIndex, int32 NumberOfSegments);
	void HideUnusedSegments(int32 StartIndex);

	void SetState(ERoadActorState NewRoadActorState);
	ERoadActorState GetState() const;

	void SetPointType(ERoadPointType NewRoadPointType);
	ERoadPointType GetPointType() const;

	TArray<FVector> GetSplinePoints() const;
	void SetSplinePoints(TArray<FVector> InSplinePoints);

	float GetWidth() const;
	void SetWidth(float InWidth);

private:
	ERoadActorState RoadActorState;
	ERoadPointType RoadPointType;
};
