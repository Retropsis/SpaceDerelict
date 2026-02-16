// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DestinationData.h"
#include "DrawManagement/Room/RoomActor.h"
#include "DrawComponent.generated.h"

struct FItemManifest;
struct FRoomFragment;
class UInventoryComponent;
class UUnlockWidget;
class UInventoryItem;
class URoomAsset;
class URoomData;
class UDoorComponent;
class UDrawingBoard;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRoomStatusChange, UInventoryItem*, RoomItem, int32, Index);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FSlotAvailabilityResult&, Result);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipStatusChanged,  UInventoryItem*, Item);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryMenuToggled,  bool, bOpen);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UDrawComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDrawComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void AddRepSubObj(UObject* SubObj);
	void ToggleDrawingBoard();
	void InitializeFromRoomData();
	void BuildPresetRooms();
	void BuildRoomPool();
	ARoomActor* SpawnRoomActor(FRoomFragment* RoomFragment) const;
	void TryDrawing(UDoorComponent* DoorComponent);

	UFUNCTION()
	void OnItemConsume(const FGameplayTag& ItemType, int32 Amount);

	UFUNCTION()
	void OnKeyConsume();

	bool IsDrawingBoardOpen() const { return bDrawingBoardOpen; }
	
	int32 GetRows() const { return Rows; }
	int32 GetColumns() const { return Columns; }
	float GetTileSize() const { return TileSize; }
	float GetRoomSize() const { return RoomSize; }
	
	// UInventoryBase* GetInventoryMenu() const { return InventoryMenu; }

	UFUNCTION(Server, Reliable)
	void Server_DrawnRoomSlotClicked(UInventoryItem* RoomToSpawn);
	
	UFUNCTION(Server, Reliable)
	void Server_OpenConnectedDoor(int32 Index, const FName& Socket);
	
	UFUNCTION(Server, Reliable)
	void Server_Redraw();
	
	// UFUNCTION(Server, Reliable)
	// void Server_AddNewItem(UItemComponent* ItemComponent, int32 StackCount, int32 Remainder);
	//
	// UFUNCTION(Server, Reliable)
	// void Server_DropItem(UInventoryItem* Item, int32 StackCount);
	
	// UFUNCTION(Server, Reliable)
	// void Server_EquippedSlottedItemClicked(UInventoryItem* ItemToEquip, UInventoryItem* ItemToUnequip);
	//
	// UFUNCTION(NetMulticast, Reliable)
	// void Multicast_EquippedSlottedItemClicked(UInventoryItem* ItemToEquip, UInventoryItem* ItemToUnequip);
	
	FRoomStatusChange OnRoomAdded;
	FRoomStatusChange OnRoomHovered;
	FRoomStatusChange OnRoomUnhovered;
	// FInventoryItemChange OnItemRemoved;
	// FNoRoomInInventory NoRoomInInventory;
	// FStackChange OnStackChange;
	// FItemEquipStatusChanged OnItemEquipped;
	// FItemEquipStatusChanged OnItemUnequipped;
	// FInventoryMenuToggled OnInventoryMenuToggled;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void InitializeDrawComponent();
	
	void ConstructDrawingBoard();
	void ConstructUnlockWidget();
	void DrawRooms();
	void OpenDrawingBoard();
	void CloseDrawingBoard();
	void OpenUnlockWidget();
	void CloseUnlockWidget();
	void DetermineLockedDoors(FDestinationAvailabilityResult& Result) const;
	
	FDestinationAvailabilityResult BuildDestinationAvailabilities(const FIntPoint& OffsetCoordinates, TMap<FName, FIntPoint> DestinationsOffsets) const;

	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<UDrawingBoard> DrawingBoardClass;

	UPROPERTY()
	TObjectPtr<UDrawingBoard> DrawingBoard;

	UPROPERTY()
	TMap<int32, TObjectPtr<ARoomActor>> SpawnedRooms; 

	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<UUnlockWidget> UnlockWidgetClass;

	UPROPERTY()
	TObjectPtr<UUnlockWidget> UnlockWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TObjectPtr<URoomData> RoomData;

	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TArray<TObjectPtr<URoomAsset>> RoomPool;

	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TArray<FItemManifest> DEBUG_RoomPool;
	
	TArray<UInventoryItem*> RoomsToDraw;
	
	TWeakObjectPtr<APlayerController> OwningController;
	TWeakObjectPtr<UDoorComponent> InteractingDoorComponent;
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
	bool bDrawingBoardOpen;
	bool bUnlockWidgetOpen;
	int32 Rows;
	int32 Columns;
	float TileSize;
	int32 NumberOfDrawnRooms;
	float RoomSize;
	float LockedDoorChance;
	int32 NumberOfRedraws;
};
