// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/DrawInventory/DrawingGrid.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "DrawManagement/Component/DrawComponent.h"
#include "DrawManagement/Utility/DrawingUtility.h"
#include "InventoryManagement/Utilities/InventoryUtility.h"
#include "Item/InventoryItem.h"
#include "Item/Fragment/FragmentTags.h"
#include "Item/Fragment/ItemFragment.h"
#include "Player/PlayerCharacterController.h"
#include "Widget/DrawInventory/GridSlot/RoomGridSlot.h"
#include "Widget/DrawInventory/SlottedRoom/SlottedRoom.h"
#include "Widget/Utiliies/WidgetUtiliies.h"

void UDrawingGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	DrawComponent = UDrawingUtility::GetDrawComponent(GetOwningPlayer());
	DrawComponent->OnRoomAdded.AddDynamic(this, &ThisClass::AddRoom);
	DrawComponent->OnRoomHovered.AddDynamic(this, &ThisClass::AddRoom);
	DrawComponent->OnRoomUnhovered.AddDynamic(this, &ThisClass::RemoveRoom);

	if (APlayerCharacterController* PC = Cast<APlayerCharacterController>(GetOwningPlayer()))
	{
		PC->OnPlayerPositionUpdated.AddDynamic(this, &ThisClass::OnPlayerPositionUpdate);
	}
	
	Rows = DrawComponent->GetRows();
	Columns = DrawComponent->GetColumns();
	TileSize = DrawComponent->GetTileSize();
	RoomSize = DrawComponent->GetRoomSize();
	ConstructGrid();

	Image_PlayerCursor->SetColorAndOpacity(PlayerColor);
}

void UDrawingGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns);

	for (int j = 0; j < Rows; j++)
	{
		for (int i = 0; i < Columns; i++)
		{
			URoomGridSlot* GridSlot = CreateWidget<URoomGridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition = FIntPoint(i, j);
			GridSlot->SetTileIndex(UWidgetUtiliies::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize(FVector2D(TileSize));
			GridCPS->SetPosition(FVector2D(TilePosition.X * TileSize, (Columns - TilePosition.Y) * TileSize)); // TODO: From Bottom to Top
			
			GridSlots.Add(GridSlot);
		}
	}
}

FDestinationAvailabilityResult UDrawingGrid::HasRoom(const FItemManifest& Manifest, const int32 RoomIndex, const int32 DestinationIndex, const int32 RoomYaw) const
{
	FDestinationAvailabilityResult Result;
	Result.RoomIndex = DestinationIndex;
	Result.DestinationYaw = RoomYaw;

	const FRoomFragment* RoomFragment = Manifest.GetFragmentOfType<FRoomFragment>();
	if (!RoomFragment) return FDestinationAvailabilityResult();

	for (TTuple<FIntPoint, FName>& Socket : RoomFragment->GetSockets())
	{
		FDestinationAvailability Availability;
		const FIntPoint RoomCoordinates = UWidgetUtiliies::GetPositionFromIndex(DestinationIndex, Columns);
		const FIntPoint ShiftedOffset = UDrawingUtility::GetShiftedOffsetFromAngle(Socket.Key, RoomYaw);
		const FIntPoint ShiftedCoordinates = RoomCoordinates + ShiftedOffset;
		Availability.Socket = Socket.Value;
		Availability.DestinationIndex = UWidgetUtiliies::GetIndexFromPositionNoWrap(ShiftedCoordinates, Columns, Rows);

		if (Availability.DestinationIndex < 0)
		{
			Availability.DoorState = EDoorState::Sealed;
		}
		else if (IsDestinationAvailable(Availability.DestinationIndex, RoomCoordinates, ShiftedCoordinates, RoomYaw))
		{
			Availability.DoorState = EDoorState::Opened;
			DrawComponent->Server_OpenConnectedDoor(Availability.DestinationIndex, Availability.Socket);
			// if (SlottedRooms.Contains(Availability.DestinationIndex) && IsValid(SlottedRooms[Availability.DestinationIndex]->GetInventoryItem()))
			// {
			// 	DrawComponent->Server_OpenConnectedDoor(SlottedRooms[Availability.DestinationIndex]->GetInventoryItem(), Availability.Socket);
			// }
			continue;
		}
		
		// if (IsIndexClaimed(Availability.DestinationIndex))
		// {
			// Check for connecting doors
		// }
		
		if (!IsInGridBounds(Availability.DestinationIndex, GetItemDimensions(Manifest)))
		{
			Availability.DoorState = EDoorState::Sealed;
		}
		
		UE_LOG(LogTemp, Warning, TEXT("From room index %d to destination index %d, Yaw %d, ShiftedOffset %s, Socket %s"),
			RoomIndex,
			Availability.DestinationIndex,
			RoomYaw,
			*ShiftedOffset.ToString(),
			*Availability.Socket.ToString());
		
		if (RoomIndex == Availability.DestinationIndex) continue;
		
		Result.DestinationAvailabilities.Add(Availability);
	}	
	return Result;
}

bool UDrawingGrid::IsDestinationAvailable(const int32 Index, const FIntPoint& RoomCoordinates, const FIntPoint& DestinationCoordinates, const int32 Yaw) const
{	
	UE_LOG(LogTemp, Warning, TEXT("Checking Destination for Room Index %d"), Index);
	if (SlottedRooms.Contains(Index))
	{
		UInventoryItem* FoundItem = SlottedRooms[Index]->GetInventoryItem();
		if (IsValid(FoundItem))
		{
			const FRoomFragment* RoomFragment = FoundItem->GetItemManifest().GetFragmentOfType<FRoomFragment>();
			UE_LOG(LogTemp, Warning, TEXT("Checking Destination Available with Yaw %d"), Yaw);
			if (!RoomFragment)
			{
				UE_LOG(LogTemp, Warning, TEXT("Has NOT found a valid RoomFragment"));
				return false;
			}

			for (const TTuple<FIntPoint, FName>& Socket : RoomFragment->GetSockets())
			{
				const FIntPoint ShiftedOffset = UDrawingUtility::GetShiftedOffsetFromAngle(Socket.Key, Yaw);
				const FIntPoint ShiftedCoordinates = DestinationCoordinates + ShiftedOffset;
				UE_LOG(LogTemp, Warning, TEXT("Checking Socket %s with ShiftedCoordinates %s from RoomCoordinates %s"), *Socket.Value.ToString(), *ShiftedCoordinates.ToString(), *RoomCoordinates.ToString());
				if (ShiftedCoordinates == RoomCoordinates)
				{
					return true;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Has NOT found a valid Item"));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Has NOT found SlottedRoom at Index %d"), Index);
	return false;
}

void UDrawingGrid::AddRoom(UInventoryItem* Item, int32 Index)
{
	// if (!MatchesCategory(Item)) return;
	//
	// FSlotAvailabilityResult Result = HasRoomForItem(Item);
	// AddItemToIndices(Result, Item);
	
	AddRoomAtIndex(Item, Index);
	UpdateGridSlots(Item, Index);
}

void UDrawingGrid::RemoveRoom(UInventoryItem* Item, int32 Index)
{
	RemoveRoomFromGrid(Item, Index);
}

void UDrawingGrid::AddRoomAtIndex(UInventoryItem* Item, const int32 Index)
{
	const FGridFragment* GridFragment = GetFragment<FGridFragment>(Item, Fragment::Grid);
	const FImageFragment* ImageFragment = GetFragment<FImageFragment>(Item, Fragment::Icon);
	const FRoomFragment* RoomFragment = GetFragment<FRoomFragment>(Item, Fragment::Room);
	if (!GridFragment || !ImageFragment || !RoomFragment) return;

	USlottedRoom* SlottedRoom = CreateSlottedRoom(Item, GridFragment, ImageFragment, Index, RoomFragment->GetYaw());
	AddSlottedRoomToCanvas(Index, GridFragment, SlottedRoom);
	SlottedRooms.Add(Index, SlottedRoom);
}

void UDrawingGrid::UpdateGridSlots(UInventoryItem* NewItem, int32 Index)
{
	check(GridSlots.IsValidIndex(Index));
	
	const FGridFragment* GridFragment = GetFragment<FGridFragment>(NewItem, Fragment::Grid);
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1);

	UInventoryUtility::ForEach2D(GridSlots, Index, Dimensions, Columns, [&] (UInventoryGridSlot* GridSlot)
	{
		GridSlot->SetInventoryItem(NewItem);
		GridSlot->SetUpperLeftIndex(Index);
		GridSlot->SetOccupiedTexture();
		GridSlot->SetAvailable(false);
	});
}

bool UDrawingGrid::IsIndexClaimed(int32 Index) const
{
	return GridSlots[Index]->IsAvailable();
}

bool UDrawingGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& Dimensions) const
{
	if (StartIndex < 0 || StartIndex >= GridSlots.Num()) return false;
	const int32 EndColumn = (StartIndex % Columns) + Dimensions.X;
	const int32 EndRow = (StartIndex / Columns) + Dimensions.Y;
	return EndColumn <= Columns && EndRow <= Rows;
}

FIntPoint UDrawingGrid::GetItemDimensions(const FItemManifest& ItemManifest) const
{
	const FGridFragment* GridFragment = ItemManifest.GetFragmentOfType<FGridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1);
}

void UDrawingGrid::RemoveRoomFromGrid(UInventoryItem* InventoryItem, const int32 GridIndex)
{
	const FGridFragment* GridFragment = GetFragment<FGridFragment>(InventoryItem, Fragment::Grid);
	if (!GridFragment) return;

	UInventoryUtility::ForEach2D(GridSlots, GridIndex, GridFragment->GetGridSize(), Columns, [&] (UInventoryGridSlot* GridSlot)
	{
		GridSlot->SetInventoryItem(nullptr);
		GridSlot->SetUpperLeftIndex(INDEX_NONE);
		GridSlot->SetUnoccupiedTexture();
		GridSlot->SetAvailable(true);
	});

	if (SlottedRooms.Contains(GridIndex))
	{
		TObjectPtr<USlottedRoom> FoundSlottedRoom;
		SlottedRooms.RemoveAndCopyValue(GridIndex, FoundSlottedRoom);
		FoundSlottedRoom->RemoveFromParent();
	}
}

USlottedRoom* UDrawingGrid::CreateSlottedRoom(UInventoryItem* Item, const FGridFragment* GridFragment, const FImageFragment* ImageFragment, const int32 Index, const int32 Angle) const
{
	USlottedRoom* SlottedRoom = CreateWidget<USlottedRoom>(GetOwningPlayer(), SlottedRoomClass);
	SlottedRoom->SetInventoryItem(Item);
	SlottedRoom->SetGridIndex(Index);
	SlottedRoom->SetImageTransformAngle(Angle);
	SetSlottedImage(SlottedRoom, GridFragment, ImageFragment);
	return SlottedRoom;
}

void UDrawingGrid::SetSlottedImage(const USlottedRoom* SlottedRoom, const FGridFragment* GridFragment, const FImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedRoom->SetImageBrush(Brush);
}

void UDrawingGrid::AddSlottedRoomToCanvas(const int32 Index, const FGridFragment* GridFragment, USlottedRoom* SlottedRoom) const
{
	CanvasPanel->AddChild(SlottedRoom);
	FVector2D CanvasSize = CanvasPanel->GetDesiredSize();
	// GEngine->AddOnScreenDebugMessage(4557, 90.f, FColor::Cyan, FString::Printf(TEXT("CanvasSize: %f / %f"), CanvasSize.X, CanvasSize.Y));
	
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedRoom);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));
	// const FVector2D DrawPosition = UWidgetUtiliies::GetPositionFromIndex(Index, Columns) * TileSize;
	// const FVector2D DrawPositionWithPadding = DrawPosition + FVector2D(GridFragment->GetGridPadding());
	const FVector2D DrawPosition = UWidgetUtiliies::GetPositionFromIndex(Index, Columns);
	const FVector2D DrawPositionWithPadding = FVector2D(DrawPosition.X * TileSize, (Columns - DrawPosition.Y) * TileSize);
	CanvasSlot->SetPosition(DrawPositionWithPadding);
}

void UDrawingGrid::OnPlayerPositionUpdate(const FVector2D& Location, const float Angle)
{
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Image_PlayerCursor);
	const float PlayerX = Location.X / RoomSize * TileSize + TileSize / 2.f;
	const float PlayerY = Columns * TileSize + Location.Y / RoomSize * TileSize + TileSize / 2.f;
	const FVector2D DrawPosition = FVector2D(PlayerX, PlayerY);
	CanvasSlot->SetPosition(DrawPosition);
	
	Image_PlayerCursor->SetRenderTransformAngle(Angle + 90.f);
}

FVector2D UDrawingGrid::GetDrawSize(const FGridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	return  GridFragment->GetGridSize() * IconTileWidth;
}

bool UDrawingGrid::IsRightClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UDrawingGrid::IsLeftClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

bool UDrawingGrid::HasValidItem(const URoomGridSlot* GridSlot) const
{
	return GridSlot->GetInventoryItem().IsValid();
}
