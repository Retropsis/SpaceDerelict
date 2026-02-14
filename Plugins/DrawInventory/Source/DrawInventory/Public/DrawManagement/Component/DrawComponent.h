// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawComponent.generated.h"

class UDrawingBoard;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FSlotAvailabilityResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipStatusChanged,  UInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryMenuToggled,  bool, bOpen);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UDrawComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDrawComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void AddRepSubObj(UObject* SubObj);
	// void ToggleInventoryMenu();
	
	// UInventoryBase* GetInventoryMenu() const { return InventoryMenu; }

	// UFUNCTION(Server, Reliable)
	// void Server_AddNewItem(UItemComponent* ItemComponent, int32 StackCount, int32 Remainder);
	//
	// UFUNCTION(Server, Reliable)
	// void Server_AddStacksToItem(UItemComponent* ItemComponent, int32 StackCount, int32 Remainder);
	//
	// UFUNCTION(Server, Reliable)
	// void Server_DropItem(UInventoryItem* Item, int32 StackCount);
	//
	// UFUNCTION(Server, Reliable)
	// void Server_ConsumeItem(UInventoryItem* Item);
	//
	// UFUNCTION(Server, Reliable)
	// void Server_EquippedSlottedItemClicked(UInventoryItem* ItemToEquip, UInventoryItem* ItemToUnequip);
	//
	// UFUNCTION(NetMulticast, Reliable)
	// void Multicast_EquippedSlottedItemClicked(UInventoryItem* ItemToEquip, UInventoryItem* ItemToUnequip);
	
	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FNoRoomInInventory NoRoomInInventory;
	FStackChange OnStackChange;
	FItemEquipStatusChanged OnItemEquipped;
	FItemEquipStatusChanged OnItemUnequipped;
	FInventoryMenuToggled OnInventoryMenuToggled;


protected:
	virtual void BeginPlay() override;
	void TryDrawing();

private:
	void ConstructDrawingBoard();
	void OpenDrawingBoard();
	void CloseDrawingBoard();
	// void SpawnDroppedItem(UInventoryItem* Item, int32 StackCount);
	
	// UPROPERTY(Replicated)
	// FInventoryFastArray InventoryList;

	bool bDrawingBoardMenuOpen;
	UPROPERTY(EditDefaultsOnly,Category="DrawInventory")
	TSubclassOf<UDrawingBoard> DrawingBoardClass;

	UPROPERTY()
	TObjectPtr<UDrawingBoard> DrawingBoard;
	
	TWeakObjectPtr<APlayerController> OwningController;
};
