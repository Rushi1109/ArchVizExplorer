// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/InteriorDesignMode.h"
#include "Actors/RoadActor.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/InteriorDataAsset.h"
#include "Widgets/InteriorDesignWidget.h"
#include "Actors/BuildingCreation/FloorActor.h"
#include "Actors/BuildingCreation/WallActor.h"
#include "Actors/BuildingCreation/RoofActor.h"

UInteriorDesignMode::UInteriorDesignMode() {}

void UInteriorDesignMode::Setup() {
	InteriorActor = nullptr;
	InteriorModeState = EInteriorModeState::Free;
	if (IsValid(WidgetRef) && !IsValid(Widget)) {
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetRef, "Interior Mode Widget");
	}

	if (UInteriorDesignWidget* InteriorDesignWidget = Cast<UInteriorDesignWidget>(Widget)) {
		InteriorDesignWidget->InteriorScrollBox->OnItemSelected.BindUObject(this, &UInteriorDesignMode::HandleInteriorAssetSelect);
	}
}

void UInteriorDesignMode::Cleanup() {
	if (IsValid(InteriorActor)) {
		if (InteriorActor->GetState() == EInteriorActorState::Previewing) {
			InteriorActor->SetState(EInteriorActorState::None);
			InteriorActor->DestroyActor();
		}
		else {
			InteriorActor->SetState(EInteriorActorState::None);
		}
		InteriorActor = nullptr;
	}
}

void UInteriorDesignMode::SetupInputMapping() {
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

		UInputAction* LeftClickAction = NewObject<UInputAction>(this);
		LeftClickAction->ValueType = EInputActionValueType::Boolean;

		UInputAction* RKeyPressAction = NewObject<UInputAction>(this);
		RKeyPressAction->ValueType = EInputActionValueType::Boolean;

		UInputAction* MKeyPressAction = NewObject<UInputAction>(this);
		MKeyPressAction->ValueType = EInputActionValueType::Boolean;

		UInputAction* DeleteKeyPressAction = NewObject<UInputAction>(this);
		DeleteKeyPressAction->ValueType = EInputActionValueType::Boolean;

		InputMappingContext = NewObject<UInputMappingContext>(this);
		InputMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);
		InputMappingContext->MapKey(RKeyPressAction, EKeys::R);
		InputMappingContext->MapKey(MKeyPressAction, EKeys::M);
		InputMappingContext->MapKey(DeleteKeyPressAction, EKeys::Delete);

		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleLeftClickAction);
		EnhancedInputComponent->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleRKeyPressAction);
		EnhancedInputComponent->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleMKeyPressAction);
		EnhancedInputComponent->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleDeleteKeyPressAction);
	}
}

void UInteriorDesignMode::EnterMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		ShowWidget();
		Setup();
	}
}

void UInteriorDesignMode::ExitMode() {
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
		}

		HideWidget();
		Cleanup();
	}
}

void UInteriorDesignMode::HandleInteriorAssetSelect(FInteriorAsset InteriorAsset) {
	if (InteriorActorRef) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		InteriorActor = GetWorld()->SpawnActor<AInteriorActor>(InteriorActorRef, SpawnParams);
		
		BindWidgetDelegates();

		InteriorActor->SetAssetType(InteriorAsset.InteriorAssetType);
		InteriorActor->SetStaticMesh(InteriorAsset.StaticMesh);
		InteriorActor->SetState(EInteriorActorState::Previewing);
		InteriorModeState = EInteriorModeState::NewEntity;
	}
}

void UInteriorDesignMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();

	if (IsValid(InteriorActor)) {
		InteriorActor->SetState(EInteriorActorState::None);
		InteriorActor = nullptr;
	}

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AInteriorActor::StaticClass())) {
		InteriorActor = Cast<AInteriorActor>(HitResult.GetActor());
		InteriorActor->SetState(EInteriorActorState::Selected);
	}
}

void UInteriorDesignMode::HandleOldEntityState() {}

void UInteriorDesignMode::HandleNewEntityState() {
	if (IsValid(InteriorActor)) {
		FHitResult HitResult = GetHitResult(TArray<AActor*>{InteriorActor});

		if (IsValid(HitResult.GetActor())) {
			if ((InteriorActor->GetAssetType() == EInteriorAssetType::FloorPlaceable && HitResult.GetActor()->IsA(AFloorActor::StaticClass())) || (InteriorActor->GetAssetType() == EInteriorAssetType::RoofPlaceable && HitResult.GetActor()->IsA(ARoofActor::StaticClass()))) {
				InteriorActor->SetActorLocation(HitResult.Location);
				InteriorActor->AttachToComponent(HitResult.GetActor()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

				InteriorActor->SetState(EInteriorActorState::Selected);
				InteriorModeState = EInteriorModeState::Free;
			}
			else if ((InteriorActor->GetAssetType() == EInteriorAssetType::WallPlaceable) && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
				InteriorActor->SetActorLocation(HitResult.Location);
				InteriorActor->SetActorRotation(HitResult.GetActor()->GetActorRotation());
				InteriorActor->AttachToComponent(HitResult.GetActor()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

				InteriorActor->SetState(EInteriorActorState::Selected);
				InteriorModeState = EInteriorModeState::Free;
			}
		}
	}
}

void UInteriorDesignMode::HandleLeftClickAction() {
	switch (InteriorModeState) {
	case EInteriorModeState::Free:
		HandleFreeState();
		break;
	case EInteriorModeState::OldEntity:
		[[fallthrough]];
	case EInteriorModeState::NewEntity:
		HandleNewEntityState();
		break;
	}
}

void UInteriorDesignMode::HandleRKeyPressAction() {
	if (IsValid(InteriorActor)) {
		InteriorActor->RotateActor(90.0);
	}
}

void UInteriorDesignMode::HandleMKeyPressAction() {
	if (IsValid(InteriorActor)) {
		InteriorActor->SetState(EInteriorActorState::Moving);
		InteriorModeState = EInteriorModeState::OldEntity;
	}
}

void UInteriorDesignMode::HandleDeleteKeyPressAction() {
	if (IsValid(InteriorActor)) {
		InteriorActor->SetState(EInteriorActorState::None);
		InteriorActor->DestroyActor();
		InteriorActor = nullptr;
	}
}

void UInteriorDesignMode::BindWidgetDelegates() {
	if (IsValid(InteriorActor) && IsValid(InteriorActor->PropertyPanel)) {
		InteriorActor->PropertyPanel->NewInteriorItemButton->OnClicked.AddDynamic(this, &UInteriorDesignMode::HandleNewButtonClick);
		InteriorActor->PropertyPanel->DeleteInteriorItemButton->OnClicked.AddDynamic(this, &UInteriorDesignMode::HandleDeleteButtonClick);
		InteriorActor->PropertyPanel->ClosePanelInteriorButton->OnClicked.AddDynamic(this, &UInteriorDesignMode::HandleClosePanelButtonClick);
	}
}

void UInteriorDesignMode::HandleNewButtonClick() {
	if (IsValid(InteriorActor)) {
		EInteriorAssetType AssetType = InteriorActor->GetAssetType();
		UStaticMesh* StaticMesh = InteriorActor->GetStaticMesh();

		InteriorActor->SetState(EInteriorActorState::None);
		InteriorActor = nullptr;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		InteriorActor = GetWorld()->SpawnActor<AInteriorActor>(InteriorActorRef, SpawnParams);

		BindWidgetDelegates();

		InteriorActor->SetAssetType(AssetType);
		InteriorActor->SetStaticMesh(StaticMesh);
		InteriorActor->SetState(EInteriorActorState::Previewing);
		InteriorModeState = EInteriorModeState::NewEntity;
	}
}

void UInteriorDesignMode::HandleDeleteButtonClick() {
	HandleDeleteKeyPressAction();
}

void UInteriorDesignMode::HandleClosePanelButtonClick() {
	if (IsValid(InteriorActor)) {
		InteriorActor->SetState(EInteriorActorState::None);
		InteriorActor = nullptr;
	}
}