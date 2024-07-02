// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/SaveAndLoadMode.h"
#include "Widgets/SaveAndLoadWidget.h"
#include "Actors/ArchVizActor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGames/ArchVizSaveGame.h"
#include "Actors/RoadActor.h"
#include "Actors/BuildingCreation/WallActor.h"
#include "Actors/BuildingCreation/FloorActor.h"
#include "Actors/BuildingCreation/RoofActor.h"
#include "Actors/BuildingCreation/DoorActor.h"
#include "Actors/InteriorActor.h"
#include "SaveGames/ArchVizSaveSlotName.h"

USaveAndLoadMode::USaveAndLoadMode() : CurrentSlotName{TEXT("")} {

}

void USaveAndLoadMode::Setup() {
	SlotsList = GetSaveSlots();

	if (IsValid(WidgetRef) && !IsValid(Widget)) {
		Widget = CreateWidget<USaveAndLoadWidget>(GetWorld(), WidgetRef, "Save And Load Widget");

		if (USaveAndLoadWidget* SaveAndLoadWidget = Cast<USaveAndLoadWidget>(Widget)) {
			SaveAndLoadWidget->PopulateSavedSlots(SlotsList);
			SaveAndLoadWidget->SaveButton->OnClicked.AddDynamic(this, &USaveAndLoadMode::HandleSaveButtonClick);
			SaveAndLoadWidget->OnSaveSlotReceived.BindUObject(this, &USaveAndLoadMode::HandleSlotLoadClick);
			SaveAndLoadWidget->OnSaveSlotDeleteReceived.BindUObject(this, &USaveAndLoadMode::HandleSlotDeleteClick);
		}
	}
}

void USaveAndLoadMode::Cleanup() {
	HideWidget();
}

void USaveAndLoadMode::SetupInputMapping() {

}

void USaveAndLoadMode::EnterMode() {
	ShowWidget();
}

void USaveAndLoadMode::ExitMode() {
	HideWidget();
}

void USaveAndLoadMode::HandleSaveButtonClick() {
	if (USaveAndLoadWidget* SaveAndLoadWidget = Cast<USaveAndLoadWidget>(Widget)) {
		FString SlotName = SaveAndLoadWidget->SaveSlotName->GetText().ToString();

		if (SlotName.IsEmpty()) {
			// TODO :: Notify the name is empty
			return;
		}

		if (GetSaveSlots().Contains(SlotName)) {
			// TODO :: Notify("This Name Already Exists. Please Choose Differnet Name");
			return;
		}

		CurrentSlotName = SlotName;
		SaveGame(CurrentSlotName);

		SaveAndLoadWidget->SaveSlotName->SetText(FText{});
	}
}

void USaveAndLoadMode::HandleSlotLoadClick(const FString& SlotName) {
	if (CurrentSlotName == SlotName) {
		// Notify("Project is Already Opened.");
		return;
	}

	CurrentSlotName = SlotName;
	LoadGame(CurrentSlotName);

	// TODO :: Success
}

void USaveAndLoadMode::HandleSlotDeleteClick(const FString& SlotName) {
	if (SlotName == CurrentSlotName) {
		// TOdo :: Notify("Can Not Delete Running Project.");
		return;
	}

	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0)) {
		if (UGameplayStatics::DeleteGameInSlot(SlotName, 0)) {
			DeleteSlotData(SlotName);

			// TODO :: Notify("Project " + SlotInfo + " Deleted");
		}
	}
}

void USaveAndLoadMode::DeleteSlotData(const FString& SlotName) {
	UArchVizSaveSlotName* ArchVizSaveSlotName = Cast<UArchVizSaveSlotName>(UGameplayStatics::LoadGameFromSlot("SavedSlotNames", 0));
	
	if (IsValid(ArchVizSaveSlotName)) {
		ArchVizSaveSlotName->SlotsName.Remove(SlotName);
		SlotsList = ArchVizSaveSlotName->SlotsName;
		UpdateWidgetWithNewSlots();

		UGameplayStatics::SaveGameToSlot(ArchVizSaveSlotName, "SavedSlotNames", 0);
	}
}

void USaveAndLoadMode::SaveGame(const FString& SlotName) {
	FString SaveDirectory = FPaths::ProjectSavedDir();
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*SaveDirectory)) {
		UE_LOG(LogTemp, Warning, TEXT("Save directory does not exist. Attempting to create: %s"), *SaveDirectory);
		if (!FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*SaveDirectory)) {
			UE_LOG(LogTemp, Error, TEXT("Failed to create save directory: %s"), *SaveDirectory);
			return;
		}
	}

	UArchVizSaveGame* SaveGameInstance = Cast<UArchVizSaveGame>(UGameplayStatics::CreateSaveGameObject(UArchVizSaveGame::StaticClass()));

	if (!IsValid(SaveGameInstance)) {
		return;
	}

	for (TActorIterator<ARoadActor> It(GetWorld()); It; ++It) {
		ARoadActor* RoadActor = *It;

		FRoadData RoadData;
		RoadData.ID = RoadActor->GetID();
		RoadData.Transform = RoadActor->GetActorTransform();
		RoadData.SplinePoints = RoadActor->GetSplinePoints();
		RoadData.PointType = RoadActor->GetPointType();
		RoadData.Width = RoadActor->GetWidth();
		RoadData.Material = RoadActor->GetMaterial();
		if (IsValid(RoadActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(RoadActor->GetAttachParentActor())) {
				RoadData.ParentActorId = ParentActor->GetID();
			}
		}
		else {
			RoadData.ParentActorId = -1;
		}
		SaveGameInstance->RoadActorArray.Add(RoadData);
	}

	for (TActorIterator<AWallActor> It(GetWorld()); It; ++It) {
		AWallActor* WallActor = *It;

		FWallData WallData;
		WallData.ID = WallActor->GetID();
		WallData.Transform = WallActor->GetActorTransform();
		WallData.Length = WallActor->GetLength();
		WallData.Material = WallActor->GetMaterial();

		if (IsValid(WallActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(WallActor->GetAttachParentActor())) {
				WallData.ParentActorId = ParentActor->GetID();
			}
		}
		else {
			WallData.ParentActorId = -1;

		}

		SaveGameInstance->WallActorArray.Add(WallData);
	}

	for (TActorIterator<AFloorActor> It(GetWorld()); It; ++It) {
		AFloorActor* FloorActor = *It;

		FFloorData FloorData;
		FloorData.ID = FloorActor->GetID();

		FloorData.Transform = FloorActor->GetActorTransform();
		FloorData.Material = FloorActor->GetMaterial();
		FloorData.Dimensions = FloorActor->GetDimensions();
		FloorData.Dimensions = FloorActor->GetOffset();
		if (IsValid(FloorActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(FloorActor->GetAttachParentActor())) {
				FloorData.ParentActorId = ParentActor->GetID();
			}
		}
		else {
			FloorData.ParentActorId = -1;

		}
		SaveGameInstance->FloorActorArray.Add(FloorData);
	}

	for (TActorIterator<ARoofActor> It(GetWorld()); It; ++It) {
		ARoofActor* RoofActor = *It;

		FRoofData RoofData;
		RoofData.ID = RoofActor->GetID();

		RoofData.Transform = RoofActor->GetActorTransform();
		RoofData.Material = RoofActor->GetMaterial();
		RoofData.Dimensions = RoofActor->GetDimensions();
		RoofData.Offset = RoofActor->GetOffset();
		if (IsValid(RoofActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(RoofActor->GetAttachParentActor())) {
				RoofData.ParentActorId = ParentActor->GetID();
			}
		}
		else {
			RoofData.ParentActorId = -1;

		}
		SaveGameInstance->RoofActorArray.Add(RoofData);
	}

	for (TActorIterator<ADoorActor> It(GetWorld()); It; ++It) {
		ADoorActor* DoorActor = *It;

		FDoorData DoorData;
		DoorData.ID = DoorActor->GetID();
		DoorData.Transform = DoorActor->GetActorTransform();
		DoorData.bIsOpen = DoorActor->bIsOpen;
		DoorData.ParentComponentIndex = DoorActor->ParentWallComponentIndex;

		if (IsValid(DoorActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(DoorActor->GetAttachParentActor())) {
				DoorData.ParentActorId = ParentActor->GetID();
			}
		}
		else {
			DoorData.ParentActorId = -1;

		}
		SaveGameInstance->DoorActorArray.Add(DoorData);
	}


	for (TActorIterator<AInteriorActor> It(GetWorld()); It; ++It) {
		AInteriorActor* InteriorActor = *It;

		FInteriorData InteriorData;
		InteriorData.ID = InteriorActor->GetID();
		InteriorData.Transform = InteriorActor->GetActorTransform();
		InteriorData.StaticMesh = InteriorActor->GetStaticMesh();
		if (IsValid(InteriorActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(InteriorActor->GetAttachParentActor())) {
				InteriorData.ParentActorId = ParentActor->GetID();
			}
		}
		else {
			InteriorData.ParentActorId = -1;
		}
		SaveGameInstance->InteriorActorArray.Add(InteriorData);
	}

	if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0)) {
		UArchVizSaveSlotName* SaveSlotNameInstance = Cast<UArchVizSaveSlotName>(UGameplayStatics::CreateSaveGameObject(UArchVizSaveSlotName::StaticClass()));

		UArchVizSaveSlotName* LoadSavedSlotNames = Cast<UArchVizSaveSlotName>(UGameplayStatics::LoadGameFromSlot(TEXT("SavedSlotNames"), 0));
		if (IsValid(LoadSavedSlotNames)) {
			SaveSlotNameInstance->SlotsName = LoadSavedSlotNames->SlotsName;
		}

		FString NewSlotName;
		NewSlotName = SlotName;
		SaveSlotNameInstance->SlotsName.Add(NewSlotName);

		SlotsList = SaveSlotNameInstance->SlotsName;
		UpdateWidgetWithNewSlots();

		UGameplayStatics::SaveGameToSlot(SaveSlotNameInstance, "SavedSlotNames", 0);
	}
}

void USaveAndLoadMode::LoadGame(const FString& SlotName) {
	UArchVizSaveGame* LoadGameInstance = Cast<UArchVizSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (LoadGameInstance) {

		ClearWholeWorld();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		TMap<int32, AActor*> IDToActorMap;
		TMap<AActor*, int32> ActorToParentActorIDMap;

		for (const FRoadData& RoadData : LoadGameInstance->RoadActorArray) {
			ARoadActor* RoadActor = GetWorld()->SpawnActor<ARoadActor>(RoadActorRef, RoadData.Transform, SpawnParams);
			RoadActor->SetActorTransform(RoadData.Transform);
			RoadActor->SetSplinePoints(RoadData.SplinePoints);
			RoadActor->SetPointType(RoadData.PointType);
			RoadActor->SetWidth(RoadData.Width);
			RoadActor->SetMaterial(RoadData.Material);
			//RoadActor->SynchronizePropertyPanel();
			RoadActor->UpdateRoad();
			//RoadActor->UnHighLightBorder();
			IDToActorMap.Add(RoadData.ID, RoadActor);

			if (RoadData.ParentActorId != -1) {
				ActorToParentActorIDMap.Add(RoadActor, RoadData.ParentActorId);
			}
		}


		for (const FWallData& WallData : LoadGameInstance->WallActorArray) {
			AWallActor* WallActor = GetWorld()->SpawnActor<AWallActor>(WallActorRef, WallData.Transform, SpawnParams);
			WallActor->SetActorTransform(WallData.Transform);
			WallActor->SetLength(WallData.Length);
			WallActor->SetMaterial(WallData.Material);
			WallActor->GenerateSegments(WallActor->GetLength());
			//WallActor->SynchronizePropertyPanel();
			IDToActorMap.Add(WallData.ID, WallActor);

			if (WallData.ParentActorId != -1) {
				ActorToParentActorIDMap.Add(WallActor, WallData.ParentActorId);
			}
		}

		for (const FFloorData& FloorData : LoadGameInstance->FloorActorArray) {
			AFloorActor* FloorActor = GetWorld()->SpawnActor<AFloorActor>(FloorActorRef, FloorData.Transform, SpawnParams);
			FloorActor->SetActorTransform(FloorData.Transform);
			FloorActor->SetDimensions(FloorData.Dimensions);
			FloorActor->SetMaterial(FloorData.Material);
			// FloorActor->SynchronizePropertyPanel();
			FloorActor->GenerateFloor(FloorData.Dimensions, FloorData.Offset);
			IDToActorMap.Add(FloorData.ID, FloorActor);

			if (FloorData.ParentActorId != -1) {
				ActorToParentActorIDMap.Add(FloorActor, FloorData.ParentActorId);
			}
		}

		for (const FRoofData& RoofData : LoadGameInstance->RoofActorArray) {
			ARoofActor* RoofActor = GetWorld()->SpawnActor<ARoofActor>(RoofActorRef, RoofData.Transform, SpawnParams);
			RoofActor->SetActorTransform(RoofData.Transform);
			RoofActor->SetDimensions(RoofData.Dimensions);
			RoofActor->SetMaterial(RoofData.Material);
			// FloorActor->SynchronizePropertyPanel();
			RoofActor->GenerateRoof(RoofData.Dimensions, RoofData.Offset);
			IDToActorMap.Add(RoofData.ID, RoofActor);

			if (RoofData.ParentActorId != -1) {
				ActorToParentActorIDMap.Add(RoofActor, RoofData.ParentActorId);
			}
		}


		for (const FDoorData& DoorData : LoadGameInstance->DoorActorArray) {
			ADoorActor* DoorActor = GetWorld()->SpawnActor<ADoorActor>(DoorActorRef, DoorData.Transform, SpawnParams);
			DoorActor->SetActorTransform(DoorData.Transform);
			DoorActor->ParentWallComponentIndex = DoorData.ParentComponentIndex;
			DoorActor->bIsOpen = DoorData.bIsOpen;
			//DoorActor->SynchronizePropertyPanel();
			IDToActorMap.Add(DoorData.ID, DoorActor);

			if (DoorData.ParentActorId != -1) {
				ActorToParentActorIDMap.Add(DoorActor, DoorData.ParentActorId);
			}
		}



		for (const FInteriorData& InteriorData : LoadGameInstance->InteriorActorArray) {
			AInteriorActor* InteriorActor = GetWorld()->SpawnActor<AInteriorActor>(InteriorActorRef, InteriorData.Transform, SpawnParams);
			InteriorActor->SetActorTransform(InteriorData.Transform);
			InteriorActor->SetStaticMesh(InteriorData.StaticMesh);
			IDToActorMap.Add(InteriorData.ID, InteriorActor);

			if (InteriorData.ParentActorId != -1) {
				ActorToParentActorIDMap.Add(InteriorActor, InteriorData.ParentActorId);
			}
		}

		for (auto& [Actor, ParentActorId] : ActorToParentActorIDMap) {
			if (auto ParentActorPtr = IDToActorMap.Find(ParentActorId)) {
				if (auto ParentActor = *ParentActorPtr; IsValid(ParentActor)) {
					if (Actor->IsA(ADoorActor::StaticClass())) {
						if (auto ParentWall = Cast<AWallActor>(ParentActor); IsValid(ParentWall)) {
							auto Door = Cast<ADoorActor>(Actor);
							if (IsValid(Door)) {
								ParentWall->AttachDoorComponent(ParentWall->WallSegments[Door->ParentWallComponentIndex], Door);
								ParentWall->UpdateSegments();
							}

						}
					}
					else {
						Actor->AttachToActor(ParentActor, FAttachmentTransformRules::KeepWorldTransform);
					}
				}

			}
		}
	}
}

void USaveAndLoadMode::UpdateWidgetWithNewSlots() {
	if (USaveAndLoadWidget* SaveAndLoadWidget = Cast<USaveAndLoadWidget>(Widget)) {
		SaveAndLoadWidget->PopulateSavedSlots(SlotsList);
	}
}

TArray<FString> USaveAndLoadMode::GetSaveSlots() {
	UArchVizSaveSlotName* SlotList = Cast<UArchVizSaveSlotName>(UGameplayStatics::LoadGameFromSlot("SavedSlotNames", 0));

	if (SlotList) {
		return SlotList->SlotsName;
	}

	return TArray<FString>();
}


void USaveAndLoadMode::ClearWholeWorld() {
	for (TActorIterator<AArchVizActor> It{GetWorld()}; It; ++It) {
		if (AArchVizActor* Actor = *It; IsValid(Actor)) {
			Actor->Destroy();
		}
	}

}