// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actors/InteriorActor.h"
#include "InteriorDataAsset.generated.h"

USTRUCT(Blueprintable)
struct FInteriorAsset {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EInteriorAssetType InteriorAssetType;
};

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UInteriorDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FInteriorAsset> InteriorAssets;
};
