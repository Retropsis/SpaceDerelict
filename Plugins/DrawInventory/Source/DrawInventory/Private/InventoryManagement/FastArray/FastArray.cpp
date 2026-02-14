
#include "InventoryManagement/FastArray/FastArray.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "Item/InventoryItem.h"

TArray<UInventoryItem*> FInventoryFastArray::GetAllItems() const
{
	TArray<UInventoryItem*> Results;
	Results.Reserve(Entries.Num());
	for (const FInv_InventoryEntry& Entry : Entries)
	{
		if (!IsValid(Entry.Item)) continue;
		Results.Add(Entry.Item);
	}
	return Results;
}

void FInventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	UInventoryComponent* IC = Cast<UInventoryComponent>(OwnerComponent);
	if (!IsValid(IC)) return;

	for (int32 Index : RemovedIndices)
	{
		IC->OnItemRemoved.Broadcast(Entries[Index].Item);
	}
}

void FInventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	UInventoryComponent* IC = Cast<UInventoryComponent>(OwnerComponent);
	if (!IsValid(IC)) return;

	for (int32 Index : AddedIndices)
	{
		IC->OnItemAdded.Broadcast(Entries[Index].Item);
	}
}

UInventoryItem* FInventoryFastArray::AddEntry(UItemComponent* ItemComponent)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	
	UInventoryComponent* IC = Cast<UInventoryComponent>(OwnerComponent);
	if (!IsValid(IC)) return nullptr;
	
	FInv_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = ItemComponent->GetItemManifest().Manifest(OwningActor);

	IC->AddRepSubObj(NewEntry.Item);

	MarkItemDirty(NewEntry);

	return NewEntry.Item;
}

UInventoryItem* FInventoryFastArray::AddEntry(UInventoryItem* Item)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FInv_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = Item;

	MarkItemDirty(NewEntry);

	return Item;
}

void FInventoryFastArray::RemoveEntry(UInventoryItem* Item)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FInv_InventoryEntry& Entry = *EntryIt;
		if (Entry.Item == Item)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UInventoryItem* FInventoryFastArray::FindFirstItemByType(const FGameplayTag& ItemType)
{
	FInv_InventoryEntry* FoundItem = Entries.FindByPredicate([ItemType = ItemType] (const FInv_InventoryEntry& Entry)
	{
		return IsValid(Entry.Item) && Entry.Item->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
	});
	return FoundItem ? FoundItem->Item : nullptr;
}
