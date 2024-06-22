// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchVizActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AArchVizActor : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArchVizActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroyActor();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FHitResult GetHitResult(const TArray<AActor*>& ActorsToIgnore) const;
};
