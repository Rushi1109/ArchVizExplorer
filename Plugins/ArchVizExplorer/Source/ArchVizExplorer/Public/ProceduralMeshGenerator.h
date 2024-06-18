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
	void GenerateCube(UProceduralMeshComponent* ProceduralMeshComponent, int32 SectionIndex, double DimensionX, double DimensionY, double DimensionZ, double ZOffset = 0.f);

	void DrawTriangleFromVertex(TArray<int32>& Triangles, int32 Vertex0, int32 Vertex1, int32 Vertex2);
};
