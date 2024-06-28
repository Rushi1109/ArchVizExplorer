// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "RoadConstructionWidget.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API URoadConstructionWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextRoadMode;
};
