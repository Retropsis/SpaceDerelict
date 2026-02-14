// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Inventory/Spatial/SpatialInventory.h"
#include "DrawInventory.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "InventoryManagement/Utilities/InventoryUtility.h"
#include "Item/InventoryItem.h"
#include "Item/Fragment/ItemFragment.h"
#include "Widget/Inventory/GridSlot/EquippedGridSlot.h"
#include "Widget/Inventory/HoverItem/HoverItem.h"
#include "Widget/Inventory/SlottedItem/EquippedSlottedItem.h"
#include "Widget/Inventory/Spatial/InventoryGrid.h"
#include "Widget/ItemDescription/ItemDescription.h"

void USpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::ShowEquippables);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::ShowCraftables);

	Grid_Equippables->SetOwningCanvas(CanvasPanel);
	Grid_Consumables->SetOwningCanvas(CanvasPanel);
	Grid_Craftables->SetOwningCanvas(CanvasPanel);

	ShowEquippables();

	WidgetTree->ForEachWidget([this] (UWidget* Widget)
	{
		if (UEquippedGridSlot* EquippedGridSlot = Cast<UEquippedGridSlot>(Widget); IsValid(EquippedGridSlot))
		{
			EquippedGridSlots.Add(EquippedGridSlot);
			EquippedGridSlot->EquippedGridSlotClicked.AddDynamic(this, &ThisClass::EquippedGridSlotClicked);
		}
	});
}

void USpatialInventory::EquippedGridSlotClicked(UEquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentType)
{
	// Check to see if we can equip the Hover Item
	if (!CanEquipHoverItem(EquippedGridSlot, EquipmentType)) return;

	// Create an Equipped Slotted Item and add it to the Equipped Grid Slot
	const float TileSize = UInventoryUtility::GetInventoryWidget(GetOwningPlayer())->GetTileSize();
	UHoverItem* HoverItem = GetHoverItem();
	UEquippedSlottedItem* EquippedSlottedItem = EquippedGridSlot->OnItemEquipped(HoverItem->GetInventoryItem(), EquipmentType, TileSize);
	EquippedSlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	
	// Inform the server that we've equipped an item (potentially unequipping an item as well)
	UInventoryComponent* InventoryComponent = UInventoryUtility::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));

	InventoryComponent->Server_EquippedSlottedItemClicked(HoverItem->GetInventoryItem(), nullptr);

	if (GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipped.Broadcast(HoverItem->GetInventoryItem());
	}
		
	// Clear the Hover Item
	Grid_Equippables->ClearHoverItem();
}

void USpatialInventory::EquippedSlottedItemClicked(UEquippedSlottedItem* EquippedSlottedItem)
{
	// Remove the Item Description
	UInventoryUtility::ItemUnhovered(GetOwningPlayer());

	if (IsValid(GetHoverItem()) && GetHoverItem()->IsStackable()) return;
	
	// Get Item to Equip
	UInventoryItem* ItemToEquip = IsValid(GetHoverItem()) ? GetHoverItem()->GetInventoryItem() : nullptr;
	
	// Get Item to Unequip
	UInventoryItem* ItemToUnequip = EquippedSlottedItem->GetInventoryItem();
		
	// Get the Equipped Grid Slot holding this item
	UEquippedGridSlot* EquippedGridSlot = FindSlotWithEquippedItem(ItemToUnequip);
	
	// Clear the equipped grid slot of this item (set its inventory item to nullptr)
	ClearSlotOfItem(EquippedGridSlot);
	
	// Assign previously equipped item as the hover item
	Grid_Equippables->AssignHoverItem(ItemToUnequip);
	
	// Remove of the equipped slotted item from the equipped grid slot
	RemoveEquippedSlottedItem(EquippedSlottedItem);
	
	// Make a new equipped slotted item (for the item we held in HoverItem)
	MakeEquippedSlottedItem(EquippedSlottedItem, EquippedGridSlot, ItemToEquip);
	
	// Broadcast delegates for OnItemEquipped/OnItemUnequipped (from the IC)
	BroadcastSlotClickedDelegates(ItemToEquip, ItemToUnequip);
}

void USpatialInventory::BroadcastSlotClickedDelegates(UInventoryItem* ItemToEquip, UInventoryItem* ItemToUnequip) const
{
	UInventoryComponent* InventoryComponent = UInventoryUtility::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));

	InventoryComponent->Server_EquippedSlottedItemClicked(ItemToEquip, ItemToUnequip);

	if (GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipped.Broadcast(ItemToEquip);
		InventoryComponent->OnItemUnequipped.Broadcast(ItemToUnequip);
	}
}

void USpatialInventory::MakeEquippedSlottedItem(UEquippedSlottedItem* EquippedSlottedItem, UEquippedGridSlot* EquippedGridSlot, UInventoryItem* ItemToEquip)
{
	if (!IsValid(EquippedGridSlot)) return;

	UEquippedSlottedItem* SlottedItem = EquippedGridSlot->OnItemEquipped(ItemToEquip, EquippedSlottedItem->GetEquipmentType(), UInventoryUtility::GetInventoryWidget(GetOwningPlayer())->GetTileSize());
	if (IsValid(SlottedItem)) SlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::USpatialInventory::EquippedSlottedItemClicked);
	EquippedGridSlot->SetEquippedSlotItem(SlottedItem);
}

void USpatialInventory::RemoveEquippedSlottedItem(UEquippedSlottedItem* EquippedSlottedItem)
{
	if (!IsValid(EquippedSlottedItem)) return;

	if (EquippedSlottedItem->OnEquippedSlottedItemClicked.IsAlreadyBound(this, &ThisClass::EquippedSlottedItemClicked))
	{
		EquippedSlottedItem->OnEquippedSlottedItemClicked.RemoveDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	}
	EquippedSlottedItem->RemoveFromParent();
}

void USpatialInventory::ClearSlotOfItem(UEquippedGridSlot* EquippedGridSlot)
{
	if (IsValid(EquippedGridSlot))
	{
		EquippedGridSlot->SetEquippedSlotItem(nullptr);
		EquippedGridSlot->SetInventoryItem(nullptr);
	}
}

UEquippedGridSlot* USpatialInventory::FindSlotWithEquippedItem(UInventoryItem* EquippedItem) const
{
	auto* FoundEquippedGridSlot = EquippedGridSlots.FindByPredicate([EquippedItem] (const UEquippedGridSlot* GridSlot)
	{
		return GridSlot->GetInventoryItem() == EquippedItem;
	});
	return FoundEquippedGridSlot ? *FoundEquippedGridSlot : nullptr;
}

bool USpatialInventory::CanEquipHoverItem(UEquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentType) const
{
	if (!IsValid(EquippedGridSlot) || EquippedGridSlot->GetInventoryItem().IsValid()) return false;

	UHoverItem* HoverItem = GetHoverItem();
	if (!IsValid(HoverItem)) return false;

	UInventoryItem* HeldItem = HoverItem->GetInventoryItem();

	return HasHoverItem() && IsValid(HeldItem) && !HoverItem->IsStackable() &&
		HeldItem->GetItemManifest().GetItemCategory() == EGridCategory::Equippable &&
			HeldItem->GetItemManifest().GetItemType().MatchesTag(EquipmentType);
}

FReply USpatialInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ActiveGrid->DropItem();
	return FReply::Handled();
}

void USpatialInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(ItemDescription)) return;
	SetItemDescriptionSizeAndPosition(ItemDescription, CanvasPanel);
	
	if (!IsValid(EquippedItemDescription)) return;
	SetEquippedItemDescriptionSizeAndPosition(ItemDescription, EquippedItemDescription, CanvasPanel);
}

void USpatialInventory::SetItemDescriptionSizeAndPosition(UItemDescription* Description, UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
	if (!IsValid(ItemDescriptionCPS)) return;

	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	ItemDescriptionCPS->SetSize(ItemDescriptionSize);

	FVector2D ClampedPosition = UWidgetUtiliies::GetClampedWidgetPosition(
		UWidgetUtiliies::GetWidgetSize(Canvas),
		ItemDescriptionSize,
		UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer())
	);
	ItemDescriptionCPS->SetPosition(ClampedPosition);
}

void USpatialInventory::SetEquippedItemDescriptionSizeAndPosition(UItemDescription* Description, UItemDescription* EquippedDescription, UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
	UCanvasPanelSlot* EquippedItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(EquippedDescription);
	if (!IsValid(ItemDescriptionCPS) || !IsValid(EquippedItemDescriptionCPS)) return;
	
	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	ItemDescriptionCPS->SetSize(ItemDescriptionSize);
	
	const FVector2D EquippedItemDescriptionSize = Description->GetBoxSize();
	EquippedItemDescriptionCPS->SetSize(EquippedItemDescriptionSize);

	FVector2D ClampedPosition = UWidgetUtiliies::GetClampedWidgetPosition(
		UWidgetUtiliies::GetWidgetSize(Canvas),
		ItemDescriptionSize,
		UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer())
	);
	ClampedPosition.X -= EquippedItemDescriptionSize.X;
	EquippedItemDescriptionCPS->SetPosition(ClampedPosition);
}

FSlotAvailabilityResult USpatialInventory::HasRoomForItem(UItemComponent* ItemComponent)
{
	switch (UInventoryUtility::GetItemCategoryFromItemComponent(ItemComponent))
	{
	case EGridCategory::Equippable:
		 return Grid_Equippables->HasRoomForItem(ItemComponent);
	case EGridCategory::Consumable:
		 return Grid_Consumables->HasRoomForItem(ItemComponent);
	case EGridCategory::Craftable:
		 return Grid_Craftables->HasRoomForItem(ItemComponent);
	default:
		UE_LOG(LogInventory, Error, TEXT("Item Component doesn't have an Item Category"));
		return FSlotAvailabilityResult();
	}
}

void USpatialInventory::OnItemHovered(UInventoryItem* Item)
{
	const auto& Manifest = Item->GetItemManifest();
	UItemDescription* ItemDescriptionWidget = GetItemDescription();
	ItemDescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(ItemDescriptionTimer);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(EquippedItemDescriptionTimer);

	FTimerDelegate ItemDescriptionDelegate;
	ItemDescriptionDelegate.BindLambda([this, Item, &Manifest, ItemDescriptionWidget] ()
	{
		GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
		Manifest.AssimilateInventoryFragments(ItemDescriptionWidget);

		// Equipped Item Description
		FTimerDelegate EquippedItemDescriptionDelegate;
		EquippedItemDescriptionDelegate.BindUObject(this, &ThisClass::ShowEquippedItemDescription, Item);
		GetOwningPlayer()->GetWorldTimerManager().SetTimer(EquippedItemDescriptionTimer, EquippedItemDescriptionDelegate, EquippedItemDescriptionDelay, false);
		
	});
	GetOwningPlayer()->GetWorldTimerManager().SetTimer(ItemDescriptionTimer, ItemDescriptionDelegate, ItemDescriptionDelay, false);
}

void USpatialInventory::OnItemUnhovered()
{
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(ItemDescriptionTimer);
	GetEquippedItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(EquippedItemDescriptionTimer);
}

UItemDescription* USpatialInventory::GetItemDescription()
{
	if (!IsValid(ItemDescription))
	{
		ItemDescription = CreateWidget<UItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
		CanvasPanel->AddChild(ItemDescription);
	}
	return ItemDescription;
}

UItemDescription* USpatialInventory::GetEquippedItemDescription()
{
	if (!IsValid(EquippedItemDescription))
	{
		EquippedItemDescription = CreateWidget<UItemDescription>(GetOwningPlayer(), EquippedItemDescriptionClass);
		CanvasPanel->AddChild(EquippedItemDescription);
	}
	return EquippedItemDescription;
}

bool USpatialInventory::HasHoverItem() const
{
	if (Grid_Equippables->HasHoverItem()) return true;
	if (Grid_Consumables->HasHoverItem()) return true;
	if (Grid_Craftables->HasHoverItem()) return true;
	return false;
}

UHoverItem* USpatialInventory::GetHoverItem() const
{
	if (!ActiveGrid.IsValid()) return nullptr;
	return ActiveGrid->GetHoverItem();
}

float USpatialInventory::GetTileSize() const
{
	return Grid_Equippables->GetTileSize();
}

void USpatialInventory::ShowEquippedItemDescription(UInventoryItem* Item)
{
	const auto& Manifest = Item->GetItemManifest();
	const FEquipmentFragment* EquipmentFragment = Manifest.GetFragmentOfType<FEquipmentFragment>();
	if (!EquipmentFragment) return;
	
	const FGameplayTag HoveredEquipmentType = EquipmentFragment->GetEquipmentType();

	auto EquippedGridSlot = EquippedGridSlots.FindByPredicate([Item] (const UEquippedGridSlot* GridSlot)
	{
		return GridSlot->GetInventoryItem() == Item;
	});
	if (EquippedGridSlot != nullptr) return; // Equipped Item is the same

	auto FoundEquippedGridSlot = EquippedGridSlots.FindByPredicate([HoveredEquipmentType] (const UEquippedGridSlot* GridSlot)
	{
		UInventoryItem* InventoryItem = GridSlot->GetInventoryItem().Get();
		return IsValid(InventoryItem) ? InventoryItem->GetItemManifest().GetFragmentOfType<FEquipmentFragment>()->GetEquipmentType() == HoveredEquipmentType : false;
	});
	UEquippedGridSlot* EquippedSlot = FoundEquippedGridSlot ? *FoundEquippedGridSlot : nullptr;
	if (!IsValid(EquippedSlot)) return; // Didn't find any item with the same equipment type

	UInventoryItem* EquippedItem = EquippedSlot->GetInventoryItem().Get();
	if (!IsValid(EquippedItem)) return;

	const auto& EquippedItemManifest = EquippedItem->GetItemManifest();
	UItemDescription* DescriptionWidget = GetEquippedItemDescription();

	auto EquippedDescriptionWidget = GetEquippedItemDescription();
	
	EquippedDescriptionWidget->Collapse();
	DescriptionWidget->SetVisibility(ESlateVisibility::HitTestInvisible);	
	EquippedItemManifest.AssimilateInventoryFragments(EquippedDescriptionWidget);
}

void USpatialInventory::ShowEquippables()
{
	SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void USpatialInventory::ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);	
}

void USpatialInventory::ShowCraftables()
{
	SetActiveGrid(Grid_Craftables, Button_Craftables);	
}

void USpatialInventory::DisableButton(UButton* Button)
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

void USpatialInventory::SetActiveGrid(UInventoryGrid* Grid, UButton* Button)
{
	if (ActiveGrid.IsValid())
	{
		ActiveGrid->HideCursor();
		ActiveGrid->OnHide();
	}
	ActiveGrid = Grid;
	if (ActiveGrid.IsValid()) ActiveGrid->ShowCursor();
	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}
