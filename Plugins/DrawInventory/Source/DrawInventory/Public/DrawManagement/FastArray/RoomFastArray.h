#pragma once

#include "CoreMinimal.h"
#include "Item/Component/ItemComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "RoomFastArray.generated.h"

class URoomAsset;
class UDrawComponent;
class UInventoryItem;

/*
 * A single entry in inventory
 */
USTRUCT(BlueprintType)
struct FRoomEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FRoomEntry() {}

private:
	friend struct FRoomFastArray;
	friend UDrawComponent;
	
	UPROPERTY()
	TObjectPtr<UInventoryItem> Item = nullptr;
};

/*
 * A list of Room items
 */
USTRUCT(BlueprintType)
struct FRoomFastArray : public FFastArraySerializer
{
	GENERATED_BODY()

	FRoomFastArray() : OwnerComponent(nullptr) {}
	FRoomFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

	TArray<UInventoryItem*> GetAllRooms() const;

	// FFastArraySerializer Contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	// FFastArraySerializer Contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FRoomEntry, FRoomFastArray>(Entries, DeltaParams, *this);
	}

	UInventoryItem* AddEntry(URoomAsset* RoomAsset);
	UInventoryItem* AddEntry(UInventoryItem* Item);
	void RemoveEntry(UInventoryItem* Item);
	UInventoryItem* FindFirstItemByType(const FGameplayTag& ItemType);
	
private:
	friend UDrawComponent;
	
	/* Replicated List of Rooms */
	UPROPERTY()
	TArray<FRoomEntry> Entries;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FRoomFastArray> : public TStructOpsTypeTraitsBase2<FRoomFastArray>
{
	enum { WithNetDeltaSerializer = true };
};