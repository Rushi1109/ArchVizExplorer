// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/TextBlock.h"
#include "InteriorDesignWidget.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UInteriorDesignWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextInteriorMode;
};
