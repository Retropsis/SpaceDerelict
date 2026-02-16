// Retropsis 2026

#include "DrawManagement/Component/DrawComponent.h"
#include "Blueprint/UserWidget.h"
#include "Data/RoomAsset.h"
#include "Data/RoomData.h"
#include "DrawManagement/Room/RoomActor.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "InventoryManagement/Utilities/InventoryUtility.h"
#include "Item/InventoryItem.h"
#include "Item/ItemTags.h"
#include "Item/Fragment/ItemFragment.h"
#include "Net/UnrealNetwork.h"
#include "Widget/DrawInventory/DrawingBoard.h"
#include "Widget/DrawInventory/Interaction/UnlockWidget.h"
#include "Widget/Utiliies/WidgetUtiliies.h"
#include "World/Level/Door/DoorComponent.h"

UDrawComponent::UDrawComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bDrawingBoardOpen = false;
}

void UDrawComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(ThisClass, RoomList);
}

void UDrawComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

void UDrawComponent::BeginPlay()
{
	Super::BeginPlay();

	InventoryComponent = UInventoryUtility::GetInventoryComponent(Cast<APlayerController>(GetOwner()));

	InitializeFromRoomData();
	ConstructDrawingBoard();
	ConstructUnlockWidget();
	BuildPresetRooms();
	BuildRoomPool();
}

void UDrawComponent::InitializeFromRoomData()
{
	checkf(IsValid(RoomData), TEXT("Please fill in RoomData Asset in DrawComponent"));
	Rows = RoomData->Rows;
	Columns = RoomData->Columns;
	TileSize = RoomData->TileSize;
	NumberOfDrawnRooms = RoomData->NumberOfDrawnRooms;
	RoomSize = RoomData->RoomSize;
	LockedDoorChance = RoomData->LockedDoorChance;
}

void UDrawComponent::BuildPresetRooms() const
{
	for (const TTuple<FIntPoint, URoomAsset*>& PresetRoom : RoomData->PresetRooms)
	{
		FRoomFragment* RoomFragment = PresetRoom.Value->RoomManifest.GetFragmentOfTypeMutable<FRoomFragment>();
		if (!RoomFragment) continue;
		
		ARoomActor* RoomActor = SpawnRoomActor(RoomFragment);
		RoomActor->SetActorLocation(FVector(PresetRoom.Key.X * RoomSize, - PresetRoom.Key.Y * RoomSize, 0.f));
		RoomActor->SetActorRotation(FRotator(0.f, 0.f, 0.f));
		TMap<FName, FIntPoint> DestinationsOffsets = RoomActor->ConstructDestinationOffsets();

		const int32 CurrentRoomIndex = UWidgetUtiliies::GetIndexFromPositionNoWrap(PresetRoom.Key, Columns, Rows);
		FDestinationAvailabilityResult Result = BuildDestinationAvailabilities(PresetRoom.Key, DestinationsOffsets);
		Result.RoomIndex = CurrentRoomIndex;
		Result.DestinationYaw = 0;
		
		RoomActor->ConstructDoors(Result);
		FItemManifest Manifest = PresetRoom.Value->RoomManifest;
		OnRoomAdded.Broadcast(Manifest.Manifest(OwningController.Get()), CurrentRoomIndex);
	}
}

FDestinationAvailabilityResult UDrawComponent::BuildDestinationAvailabilities(const FIntPoint& OffsetCoordinates, TMap<FName, FIntPoint> DestinationsOffsets) const
{
	FDestinationAvailabilityResult Result;
	for (const TTuple<FName, FIntPoint>& DestinationsOffset : DestinationsOffsets)
	{
		FDestinationAvailability Availability;
		Availability.Socket = DestinationsOffset.Key;
		Availability.DoorState = EDoorState::Closed;
		FIntPoint NewCoordinates = OffsetCoordinates + DestinationsOffset.Value;
		Availability.DestinationIndex = UWidgetUtiliies::GetIndexFromPositionNoWrap(NewCoordinates, Columns, Rows);
		// UE_LOG(LogTemp, Warning, TEXT("Socket: %s, %s, %d"), *DestinationsOffset.Key.ToString(), *DestinationsOffset.Value.ToString(), Availability.DestinationIndex);
		Result.DestinationAvailabilities.Add(Availability);
	}
	return Result;
}

void UDrawComponent::BuildRoomPool()
{
	for (int32 i = 0; i < Columns * Rows; ++i)
	{
		const int32 Selection = FMath::RandRange(0, RoomData->Rooms.Num() - 1);
		RoomPool.Add(RoomData->Rooms[Selection]);
	}
}

void UDrawComponent::Server_DrawnRoomSlotClicked_Implementation(UInventoryItem* RoomToSpawn)
{
	if (!IsValid(RoomToSpawn)) return;

	if (const FRequirementFragment* RequirementFragment = RoomToSpawn->GetItemManifest().GetFragmentOfType<FRequirementFragment>())
	{
		if (!InventoryComponent->CheckItemOfTypAndAmount(RequirementFragment->GetItemType(), RequirementFragment->GetAmount()))
		{
			return;
		}
		InventoryComponent->Server_ConsumeItemOfTypAndAmount(RequirementFragment->GetItemType(), RequirementFragment->GetAmount());
	}

	const int32 RoomIndex = InteractingDoorComponent->GetRoomIndex();
	const int32 DestinationIndex = InteractingDoorComponent->GetDestinationIndex();
	const float RoomYaw = InteractingDoorComponent->GetRoomYaw();
	const FIntPoint Coordinates = UWidgetUtiliies::GetPositionFromIndex(DestinationIndex, Columns);
	FDestinationAvailabilityResult Result = DrawingBoard->HasRoom(RoomToSpawn->GetItemManifest(), RoomIndex, DestinationIndex, RoomYaw);
	DetermineLockedDoors(Result);
	
	// UInventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());
	// Result.Item = FoundItem;
	//
	// if (Result.TotalRoomToFill == 0)
	// {
	// 	NoRoomInInventory.Broadcast();
	// 	return;
	// }
	//
	// if (Result.Item.IsValid() && Result.bStackable)
	// {
	// 	// Add Stacks to existing Item
	// 	OnStackChange.Broadcast(Result);
	// 	Server_AddStacksToItem(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
	// }
	// else if (Result.TotalRoomToFill > 0)
	// {
	// 	// Create new item
	// 	Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0, Result.Remainder);
	// }
	
	FRoomFragment* RoomFragment = RoomToSpawn->GetItemManifestMutable().GetFragmentOfTypeMutable<FRoomFragment>();
	if (!RoomFragment) return;

	RoomFragment->SetYaw(RoomYaw);
		
	ARoomActor* RoomActor = SpawnRoomActor(RoomFragment);
	RoomActor->SetActorLocation(FVector(Coordinates.X * RoomSize,  - Coordinates.Y * RoomSize, 0.f));
	RoomActor->SetActorRotation(FRotator(0.f, RoomYaw, 0.f));
	RoomActor->ConstructDestinationOffsets();
	RoomActor->ConstructDoors(Result);

	OnRoomAdded.Broadcast(RoomToSpawn, DestinationIndex);
	InteractingDoorComponent->ToggleDoor(true);
	CloseDrawingBoard();
}

void UDrawComponent::DetermineLockedDoors(FDestinationAvailabilityResult& Result) const
{
	if (Result.DestinationAvailabilities.Num() < 2) return;
	
	for (FDestinationAvailability& Availability : Result.DestinationAvailabilities)
	{
		const float Chance = FMath::FRandRange(0.f, 100.f);
		UE_LOG(LogTemp, Warning, TEXT("Rolled a %f  and chance is %f"), Chance, LockedDoorChance);
		if (Chance <= LockedDoorChance)
		{
			Availability.DoorState = EDoorState::Locked;
			return;
		}
	}
}

ARoomActor* UDrawComponent::SpawnRoomActor(FRoomFragment* RoomFragment) const
{
	ARoomActor* SpawnedRoomActor = RoomFragment->SpawnRoomActor(OwningController.Get());
	SpawnedRoomActor->SetRoomType(RoomFragment->GetRoomType());
	SpawnedRoomActor->SetOwner(GetOwner());
	RoomFragment->SetSpawnedRoomActor(SpawnedRoomActor);
	return SpawnedRoomActor;
}

void UDrawComponent::ConstructDrawingBoard()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("DrawComponent should have PlayerController as Owner."));
	if (!OwningController->IsLocalController()) return;

	DrawingBoard = CreateWidget<UDrawingBoard>(OwningController.Get(), DrawingBoardClass);
	DrawingBoard->AddToViewport();
	CloseDrawingBoard();
}

void UDrawComponent::ConstructUnlockWidget()
{
	UnlockWidget = CreateWidget<UUnlockWidget>(OwningController.Get(), UnlockWidgetClass);
	UnlockWidget->AddToViewport();
	UnlockWidget->UnlockButtonClicked.AddDynamic(this, &ThisClass::OnKeyConsume);
	CloseUnlockWidget();
}

void UDrawComponent::OnItemConsume(const FGameplayTag& ItemType, int32 Amount)
{
	InventoryComponent->Server_ConsumeItemOfTypAndAmount(ItemType, Amount);
}

void UDrawComponent::OnKeyConsume()
{
	OnItemConsume(Item::Currency::Key, 1);
	
	ToggleDrawingBoard();
	DrawRooms();
}

void UDrawComponent::TryDrawing(UDoorComponent* DoorComponent)
{
	InteractingDoorComponent = DoorComponent;

	if (InteractingDoorComponent->IsLocked())
	{
		if (InventoryComponent->CheckItemOfTypAndAmount(Item::Currency::Key, 1))
		{
			OpenUnlockWidget();
		}
	}
	else
	{
		ToggleDrawingBoard();
        DrawRooms();
	}
}

void UDrawComponent::DrawRooms()
{
	RoomsToDraw.Empty();
	for (int32 i = 0; i < NumberOfDrawnRooms; ++i)
	{
		const int32 Selection = FMath::RandRange(0, RoomPool.Num() - 1);
		// if (!IsValid(DEBUG_RoomPool[Selection])) continue;

		// RoomsToDraw.Add(RoomPool[Selection]->RoomManifest.Manifest(OwningController.Get()));
		const int32 DEBUG_Selection = FMath::RandRange(0, DEBUG_RoomPool.Num() - 1);
		FItemManifest Manifest = DEBUG_RoomPool[DEBUG_Selection];
		
		RoomsToDraw.Add(Manifest.Manifest(OwningController.Get()));
		RoomPool.RemoveAt(Selection);
		RoomPool.Shrink();
	}
	DrawingBoard->ClearDrawingBoard();
	for (UInventoryItem* Room : RoomsToDraw)
	{
		bool bRequirementMet = false;
		if (const FRequirementFragment* RequirementFragment = Room->GetItemManifest().GetFragmentOfType<FRequirementFragment>())
		{
			bRequirementMet = InventoryComponent->CheckItemOfTypAndAmount(RequirementFragment->GetItemType(), RequirementFragment->GetAmount());
		}
		DrawingBoard->DrawRoom(Room, InteractingDoorComponent->GetDestinationIndex(), InteractingDoorComponent->GetRoomYaw(), bRequirementMet);
	}
}

void UDrawComponent::ToggleDrawingBoard()
{
	if (bDrawingBoardOpen)
	{
		CloseDrawingBoard();
	}
	else
	{
		OpenDrawingBoard();
	}
	// OnInventoryMenuToggled.Broadcast(bDrawingBoardOpen);
}

void UDrawComponent::OpenDrawingBoard()
{
	if (!IsValid(DrawingBoard)) return;

	DrawingBoard->SetVisibility(ESlateVisibility::Visible);
	bDrawingBoardOpen = true;

	if (!OwningController.IsValid()) return;

	FInputModeUIOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UDrawComponent::CloseDrawingBoard()
{
	if (!IsValid(DrawingBoard)) return;
	
	DrawingBoard->SetVisibility(ESlateVisibility::Collapsed);
	bDrawingBoardOpen = false;

	if (!OwningController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
}

void UDrawComponent::OpenUnlockWidget()
{
	if (!IsValid(UnlockWidget)) return;

	UnlockWidget->SetVisibility(ESlateVisibility::Visible);
	bUnlockWidgetOpen = true;

	if (!OwningController.IsValid()) return;

	FInputModeUIOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UDrawComponent::CloseUnlockWidget()
{
	if (!IsValid(UnlockWidget)) return;

	UnlockWidget->SetVisibility(ESlateVisibility::Collapsed);
	bUnlockWidgetOpen = false;

	if (!OwningController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
}

