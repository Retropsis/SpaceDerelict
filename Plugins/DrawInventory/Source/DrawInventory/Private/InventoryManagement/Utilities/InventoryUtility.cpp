// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManagement/Utilities/InventoryUtility.h"
#include "Types/GridTypes.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "Widget/Inventory/InventoryBase/InventoryBase.h"

UInventoryComponent* UInventoryUtility::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;

	UInventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UInventoryComponent>();
	return InventoryComponent;
}

EGridCategory UInventoryUtility::GetItemCategoryFromItemComponent(const UItemComponent* ItemComponent)
{
	if (!IsValid(ItemComponent)) return EGridCategory::None;
	return ItemComponent->GetItemManifest().GetItemCategory();
}

void UInventoryUtility::ItemHovered(APlayerController* PC, UInventoryItem* Item)
{
	UInventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return;

	UInventoryBase* InventoryBase = IC->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return;

	if (InventoryBase->HasHoverItem()) return;

	InventoryBase->OnItemHovered(Item);
}

void UInventoryUtility::ItemUnhovered(APlayerController* PC)
{
	UInventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return;

	UInventoryBase* InventoryBase = IC->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return;

	InventoryBase->OnItemUnhovered();
}

UHoverItem* UInventoryUtility::GetHoverItem(APlayerController* PC)
{	
	UInventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return nullptr;

	UInventoryBase* InventoryBase = IC->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return nullptr;

	return InventoryBase->GetHoverItem();
}

UInventoryBase* UInventoryUtility::GetInventoryWidget(APlayerController* PC)
{
	UInventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return nullptr;

	return  IC->GetInventoryMenu();
}
