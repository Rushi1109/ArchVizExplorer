// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Components/Button.h"
#include "RoadConstructionWidget.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API URoadConstructionWidget : public UUserWidget {
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* NewRoadSegmentButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* CompleteCurrentSegmentButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* UndoButton;
};
