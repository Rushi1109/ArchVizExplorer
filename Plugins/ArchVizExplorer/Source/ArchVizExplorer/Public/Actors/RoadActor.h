// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizActor.h"
#include "GameFramework/Actor.h"
#include "RoadActor.generated.h"

class USplineMeshComponent;
class USplineComponent;

UCLASS()
class ARCHVIZEXPLORER_API ARoadActor : public AArchVizActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoadActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Road")
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Road")
	TArray<USplineMeshComponent*> RoadComponents;

	UPROPERTY(EditDefaultsOnly, Category = "Road")
    UStaticMesh* RoadMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void GenerateRoadMesh();

	void GenerateRoadSegment(const FVector& StartLocation, const FVector& StartTangent, const FVector& EndLocation, const FVector& EndTangent);

	UFUNCTION(BlueprintCallable)
    void DestroyRoadMesh();

	void AddNewPoint(const FVector& Location);
};
