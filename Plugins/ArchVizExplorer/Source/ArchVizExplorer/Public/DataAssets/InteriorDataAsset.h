// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actors/InteriorActor.h"
#include "InteriorDataAsset.generated.h"

USTRUCT(Blueprintable)
struct FInteriorAsset {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	EInteriorAssetType InteriorAssetType;
};

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UInteriorDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	TArray<FInteriorAsset> InteriorAssets;
};
