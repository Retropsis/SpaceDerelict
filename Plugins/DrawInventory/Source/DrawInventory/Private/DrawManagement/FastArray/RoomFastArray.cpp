
#include "DrawManagement/FastArray/RoomFastArray.h"
#include "Data/RoomAsset.h"
#include "DrawManagement/Component/DrawComponent.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "Item/InventoryItem.h"

TArray<UInventoryItem*> FRoomFastArray::GetAllRooms() const
{
	TArray<UInventoryItem*> Results;
	Results.Reserve(Entries.Num());
	for (const FRoomEntry& Entry : Entries)
	{
		if (!IsValid(Entry.Item)) continue;
		Results.Add(Entry.Item);
	}
	return Results;
}

void FRoomFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	UDrawComponent* DC = Cast<UDrawComponent>(OwnerComponent);
	if (!IsValid(DC)) return;

	for (int32 Index : RemovedIndices)
	{
		DC->OnRoomRemoved.Broadcast(Entries[Index].Item);
	}
}

void FRoomFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	UDrawComponent* DC = Cast<UDrawComponent>(OwnerComponent);
	if (!IsValid(DC)) return;

	for (int32 Index : AddedIndices)
	{
		DC->OnRoomAdded.Broadcast(Entries[Index].Item, Index);
	}
}

UInventoryItem* FRoomFastArray::AddEntry(URoomAsset* RoomAsset)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	
	UDrawComponent* DC = Cast<UDrawComponent>(OwnerComponent);
	if (!IsValid(DC)) return nullptr;
	
	FRoomEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = RoomAsset->GetRoomManifest().Manifest(OwningActor);

	DC->AddRepSubObj(NewEntry.Item);

	MarkItemDirty(NewEntry);

	return NewEntry.Item;
}

UInventoryItem* FRoomFastArray::AddEntry(UInventoryItem* Item)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FRoomEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = Item;

	MarkItemDirty(NewEntry);

	return Item;
}

void FRoomFastArray::RemoveEntry(UInventoryItem* Item)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FRoomEntry& Entry = *EntryIt;
		if (Entry.Item == Item)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UInventoryItem* FRoomFastArray::FindFirstItemByType(const FGameplayTag& ItemType)
{
	FRoomEntry* FoundItem = Entries.FindByPredicate([ItemType = ItemType] (const FRoomEntry& Entry)
	{
		return IsValid(Entry.Item) && Entry.Item->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
	});
	return FoundItem ? FoundItem->Item : nullptr;
}
