// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widgets/PropertyPanelWidget.h"
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

	void ShowPropertyPanel();
	void HidePropertyPanel();

	void HighlightSelectedActor();
	void UnHighlightDeselectedActor();

	void RotateActor(double Degrees);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArchVizActor | PropertyWidget")
	TSubclassOf<UPropertyPanelWidget> PropertyPanelRef;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ArchVizActor | PropertyWidget")
	UPropertyPanelWidget* PropertyPanel;

	int32 ActorID;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FHitResult GetHitResult(const TArray<AActor*>& ActorsToIgnore = TArray<AActor*>{}) const;

	int32 GetID() const;

private:
	static inline int32 ID = 0;

	static int GenerateID() {
		return ++ID;
	}
};
