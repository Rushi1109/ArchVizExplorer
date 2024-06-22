// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorActor.generated.h"

class UProceduralMeshComponent;

UCLASS()
class ARCHVIZEXPLORER_API AFloorActor : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Floor")
	UProceduralMeshComponent* ProceduralMeshComponent;

	FVector StartPoint;
	FVector EndPoint;

	bool bIsDragging;
};
