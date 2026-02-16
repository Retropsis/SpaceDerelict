// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManagement/FastArray/FastArray.h"
#include "InventoryComponent.generated.h"

class UItemComponent;
class UInventoryItem;
class UInventoryBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNoItemOfTypeInInventory, const FName&, ItemName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FSlotAvailabilityResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipStatusChanged,  UInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryMenuToggled,  bool, bOpen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemStackChange, const FGameplayTag&, ItemType, int32, Amount);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void AddRepSubObj(UObject* SubObj);
	void ToggleInventoryMenu();
	UInventoryBase* GetInventoryMenu() const { return InventoryMenu; }
	bool IsMenuOpen() const { return bInventoryMenuOpen; }
	bool CheckItemOfTypAndAmount(const FGameplayTag& ItemType, const int32 Amount);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_DropItem(UInventoryItem* Item, int32 StackCount);

	UFUNCTION(Server, Reliable)
	void Server_ConsumeItem(UInventoryItem* Item);
	
	UFUNCTION(Server, Reliable)
	void Server_ConsumeItemOfTypAndAmount(const FGameplayTag& ItemType, const int32 Amount);

	UFUNCTION(Server, Reliable)
	void Server_EquippedSlottedItemClicked(UInventoryItem* ItemToEquip, UInventoryItem* ItemToUnequip);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EquippedSlottedItemClicked(UInventoryItem* ItemToEquip, UInventoryItem* ItemToUnequip);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	void TryAddItem(UItemComponent* ItemComponent);
	
	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FNoRoomInInventory NoRoomInInventory;
	FStackChange OnStackChange;
	FItemEquipStatusChanged OnItemEquipped;
	FItemEquipStatusChanged OnItemUnequipped;
	FInventoryMenuToggled OnInventoryMenuToggled;
	FNoItemOfTypeInInventory NoItemOfTypeInInventory;
	FItemStackChange OnItemStackChange;

protected:
	virtual void BeginPlay() override;

private:
	void ConstructInventory();
	void OpenInventoryMenu();
	void CloseInventoryMenu();
	void SpawnDroppedItem(UInventoryItem* Item, int32 StackCount);

	TWeakObjectPtr<APlayerController> OwningController;

	UPROPERTY(Replicated)
	FInventoryFastArray InventoryList;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UInventoryBase> InventoryMenuClass;

	UPROPERTY()
	TObjectPtr<UInventoryBase> InventoryMenu;

	bool bInventoryMenuOpen;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnAngleMin = -85.f;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnAngleMax = 85.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnDistanceMin = 75.f;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnDistanceMax = 150.f;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	float RelativeSpawnElevation = 70.f;
};
