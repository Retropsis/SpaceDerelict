// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/DestinationData.h"
#include "Data/GridTypes.h"
#include "DrawingGrid.generated.h"

class UImage;
struct FItemManifest;
struct FImageFragment;
struct FGridFragment;
class URoomGridSlot;
class USlottedRoom;
class UInventoryItem;
class USlottedItem;
class UDrawComponent;
class UCanvasPanel;


UCLASS()
class DRAWINVENTORY_API UDrawingGrid : public UUserWidget
{
	GENERATED_BODY()

	public:
	virtual void NativeOnInitialized() override;
	FDestinationAvailabilityResult HasRoom(const FItemManifest& Manifest, int32 RoomIndex, int32 DestinationIndex, int32 RoomYaw) const;

	UFUNCTION()
	void AddRoom(UInventoryItem* Item, int32 Index);
	
	UFUNCTION()
	void RemoveRoom(UInventoryItem* Item, int32 Index);
	
	// UFUNCTION()
	// void AddStacks(const FSlotAvailabilityResult& Result);
	//
	// UFUNCTION()
	// void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);
	//
	// UFUNCTION()
	// void OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent);
	//
	// UFUNCTION()
	// void OnInventoryMenuToggled(bool bOpen);
	// EGridCategory GetItemCategory() const { return ItemCategory; }
	// void ShowCursor();
	// void HideCursor();
	// void SetOwningCanvas(UCanvasPanel* OwningCanvas);
	// float GetTileSize() const { return TileSize; }
	// void OnHide();

private:
	void ConstructGrid();
	bool IsRightClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftClick(const FPointerEvent& MouseEvent) const;
	bool HasValidItem(const URoomGridSlot* GridSlot) const;
	void UpdateGridSlots(UInventoryItem* NewItem, int32 Index);
	void AddRoomAtIndex(UInventoryItem* Item, const int32 Index);
	void RemoveRoomFromGrid(UInventoryItem* InventoryItem, const int32 GridIndex);
	USlottedRoom* CreateSlottedRoom(UInventoryItem* Item, const FGridFragment* GridFragment, const FImageFragment* ImageFragment, const int32 Index, int32 Angle) const;
	void SetSlottedImage(const USlottedRoom* SlottedRoom, const FGridFragment* GridFragment, const FImageFragment* ImageFragment) const;
	void AddSlottedRoomToCanvas(const int32 Index, const FGridFragment* GridFragment, USlottedRoom* SlottedRoom) const;
	FVector2D GetDrawSize(const FGridFragment* GridFragment) const;
	bool IsIndexClaimed(int32 Index) const;
	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& Dimensions) const;
	FIntPoint GetItemDimensions(const FItemManifest& ItemManifest) const;
	bool IsDestinationAvailable(const int32 Index, const FIntPoint& RoomCoordinates, const FIntPoint& DestinationCoordinates, int32 Yaw) const;

	UFUNCTION()
	void OnPlayerPositionUpdate(const FVector2D& Location, const float Angle);
	
	// bool IsUpperLeftSlot(const UInventoryGridSlot* GridSlot, const UInventoryGridSlot* SubGridSlot) const;
	// bool DoesItemTypeMatch(const UInventoryItem* SubItem, const FGameplayTag& ItemType) const;
	// bool HasRoomAtIndex(const UInventoryGridSlot* GridSlot, const FIntPoint& Dimensions, const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed, const FGameplayTag& ItemType, int32 MaxStackSize);
	// bool MatchesCategory(const UInventoryItem* Item) const;
	// FSlotAvailabilityResult HasRoomForItem(const UInventoryItem* InventoryItem, const int32 StackAmountOverride = -1);
	// FSlotAvailabilityResult HasRoomForItem(const FItemManifest& ItemManifest, int32 StackAmountOverride = -1);
	// void AddItemToIndices(const FSlotAvailabilityResult& Result, UInventoryItem* NewItem);
	// bool CheckSlotConstrains(const UInventoryGridSlot* GridSlot, const UInventoryGridSlot* SubGridSlot, const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed, const FGameplayTag& ItemType, int32 MaxStackSize) const;
	
	TWeakObjectPtr<UDrawComponent> DrawComponent;
	TWeakObjectPtr<UCanvasPanel> OwningCanvasPanel;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<URoomGridSlot> GridSlotClass;

	UPROPERTY()
	TArray<TObjectPtr<URoomGridSlot>> GridSlots;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	EGridCategory GridCategory;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY()
	TMap<int32, TObjectPtr<USlottedRoom>> SlottedRooms;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<USlottedRoom> SlottedRoomClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_PlayerCursor;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FLinearColor PlayerColor;
	
	int32 Rows;
	int32 Columns;
	float TileSize;
	float RoomSize;
};

