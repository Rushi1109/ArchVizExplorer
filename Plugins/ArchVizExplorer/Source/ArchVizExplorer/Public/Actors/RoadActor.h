// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizActor.h"
#include "GameFramework/Actor.h"
#include "RoadActor.generated.h"

class USplineMeshComponent;
class USplineComponent;

enum class ERoadActorState : uint8 {
	None,
	Generating,
	Selected
};

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
	void GenerateRoadMesh();

	void GenerateRoadSegment(const FVector& StartLocation, const FVector& StartTangent, const FVector& EndLocation, const FVector& EndTangent);

	UFUNCTION(BlueprintCallable)
    void DestroyRoadMesh();

	void AddNewPoint(const FVector& Location);
	bool RemoveLastSplinePoint();

	void UpdateRoad();

	void SetState(ERoadActorState NewRoadActorState);
	ERoadActorState GetState() const;

	void SetPointType(ERoadPointType NewRoadPointType);
	ERoadPointType GetPointType() const;

private:
	ERoadActorState RoadActorState;
	ERoadPointType RoadPointType;
};
