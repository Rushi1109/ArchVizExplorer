// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UObject/NoExportTypes.h"
#include "ArchVizMode.generated.h"

class APlayerController;
class UInputMappingContext;

/**
 *
 */
UCLASS(ABSTRACT)
class ARCHVIZEXPLORER_API UArchVizMode : public UObject {
	GENERATED_BODY()

public:

	virtual void Setup() PURE_VIRTUAL(UArchVizMode::Setup(), );
	virtual void SetupInputMapping() PURE_VIRTUAL(UArchVizMode::SetupInputMapping(), );
	virtual void EnterMode() PURE_VIRTUAL(UArchVizMode::EnterMode(), );
	virtual void ExitMode() PURE_VIRTUAL(UArchVizMode::ExitMode(), );

	virtual void InitParam(APlayerController* Controller);
	virtual void ShowWidget();
	virtual void HideWidget();

protected:
	FHitResult GetHitResult(const TArray<AActor*>& ActorsToIgnore = TArray<AActor*>{}) const;
	
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;

	//Widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WidgetRef;

	UPROPERTY()
	UUserWidget* Widget;
};
