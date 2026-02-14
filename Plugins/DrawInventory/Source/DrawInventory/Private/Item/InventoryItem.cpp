// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InventoryItem.h"

#include "Item/Fragment/ItemFragment.h"
#include "Net/UnrealNetwork.h"

void UInventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
	DOREPLIFETIME(ThisClass, TotalStackCount);
}

void UInventoryItem::SetItemManifest(const FItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FItemManifest>(Manifest);
}

bool UInventoryItem::IsStackable() const
{
	const FStackableFragment* StackableFragment = GetItemManifest().GetFragmentOfType<FStackableFragment>();
	return StackableFragment != nullptr;
}

bool UInventoryItem::IsConsumable() const
{
	return GetItemManifest().GetItemCategory() == EGridCategory::Consumable;
}
