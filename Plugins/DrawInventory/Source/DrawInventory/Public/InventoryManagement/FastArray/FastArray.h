#pragma once

#include "CoreMinimal.h"
#include "Item/Component/ItemComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "FastArray.generated.h"

class UInventoryComponent;
class UInventoryItem;

/*
 * A single entry in inventory
 */
USTRUCT(BlueprintType)
struct FInv_InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInv_InventoryEntry() {}

private:
	friend struct FInventoryFastArray;
	friend UInventoryComponent;
	
	UPROPERTY()
	TObjectPtr<UInventoryItem> Item = nullptr;
};

/*
 * A list of inventory items
 */
USTRUCT(BlueprintType)
struct FInventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()

	FInventoryFastArray() : OwnerComponent(nullptr) {}
	FInventoryFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

	TArray<UInventoryItem*> GetAllItems() const;

	// FFastArraySerializer Contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	// FFastArraySerializer Contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FInv_InventoryEntry, FInventoryFastArray>(Entries, DeltaParams, *this);
	}

	UInventoryItem* AddEntry(UItemComponent* ItemComponent);
	UInventoryItem* AddEntry(UInventoryItem* Item);
	void RemoveEntry(UInventoryItem* Item);
	UInventoryItem* FindFirstItemByType(const FGameplayTag& ItemType);
	
private:
	friend UInventoryComponent;
	
	/* Replicated List of Items */
	UPROPERTY()
	TArray<FInv_InventoryEntry> Entries;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FInventoryFastArray> : public TStructOpsTypeTraitsBase2<FInventoryFastArray>
{
	enum { WithNetDeltaSerializer = true };
};