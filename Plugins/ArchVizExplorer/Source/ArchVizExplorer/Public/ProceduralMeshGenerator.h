// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UProceduralMeshComponent;

/**
 *
 */
class ARCHVIZEXPLORER_API ProceduralMeshGenerator {
public:
	ProceduralMeshGenerator();
	~ProceduralMeshGenerator();

	UFUNCTION(BlueprintCallable)
	static void GenerateCube(UProceduralMeshComponent* ProceduralMeshComponent, int32 SectionIndex, const FVector& Dimension, const FVector& Offset = FVector{0.0});
	static void DrawTriangleFromVertex(TArray<int32>& Triangles, int32 Vertex0, int32 Vertex1, int32 Vertex2);
};
