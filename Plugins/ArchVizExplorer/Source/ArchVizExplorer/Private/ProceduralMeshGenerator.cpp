// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

ProceduralMeshGenerator::ProceduralMeshGenerator() {
}

ProceduralMeshGenerator::~ProceduralMeshGenerator() {
}

void ProceduralMeshGenerator::GenerateCube(UProceduralMeshComponent* ProceduralMeshComponent, int32 SectionIndex, const FVector& Dimension, const FVector& Offset) {
	ProceduralMeshComponent->ClearAllMeshSections();

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;
	TArray<FVector> Normals;

	// Front Face
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 0 - - +
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 1 - - -
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 2 - + -
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 3 - + +

	// Right Face
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 4 - + + 3
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 5 - + - 2
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 6 + + -
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 7 + + +

	// Back Face
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 8 + + + 7
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 9 + + - 6
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 10 + - - 
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 11 + - + 

	// Left Face
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 12 + - + 11
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 13 + - - 10
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 14 - - - 1
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 15 - - + 0

	// Top Face
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 16 + - + 11
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 17 - - + 0
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 18 - + + 3
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z + Dimension.Z / 2 }); // 19 + + + 7

	// Bottom Face
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 20 - - - 1
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y - Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 21 + - - 10
	Vertices.Add(FVector{ Offset.X + Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 22 + + - 6
	Vertices.Add(FVector{ Offset.X - Dimension.X / 2, Offset.Y + Dimension.Y / 2, Offset.Z - Dimension.Z / 2 }); // 23 - + - 2

	// Triangles
	for (int32 i = 0; i < 24; i += 4) {
		DrawTriangleFromVertex(Triangles, i, i + 1, i + 2);
		DrawTriangleFromVertex(Triangles, i + 2, i + 3, i);
	}

	// UVs
	for (int32 i = 0; i < Vertices.Num(); i += 4) {
		UVs.Add(FVector2D{ 0.0, 0.0 });
		UVs.Add(FVector2D{ 0.0, 1.0 });
		UVs.Add(FVector2D{ 1.0, 1.0 });
		UVs.Add(FVector2D{ 1.0, 0.0 });
	}

	// Normals
	for (int32 i = 0; i < 24; i += 4) {
		FVector CurrentNormal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 2], Vertices[i + 1]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 1], Vertices[i]));

		Normals.Add(CurrentNormal);
		Normals.Add(CurrentNormal);
		Normals.Add(CurrentNormal);
		Normals.Add(CurrentNormal);
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(SectionIndex, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>{}, TArray<FProcMeshTangent>{}, true);
}

void ProceduralMeshGenerator::DrawTriangleFromVertex(TArray<int32>& Triangles, int32 Vertex0, int32 Vertex1, int32 Vertex2) {
	Triangles.Add(Vertex0);
	Triangles.Add(Vertex1);
	Triangles.Add(Vertex2);
}
