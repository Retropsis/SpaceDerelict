// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManagement/Component/InventoryComponent.h"
#include "Data/GridTypes.h"
#include "Item/InventoryItem.h"
#include "Item/Fragment/ItemFragment.h"
#include "Net/UnrealNetwork.h"
#include "Widget/Inventory/InventoryBase/InventoryBase.h"

UInventoryComponent::UInventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructInventory();
}

void UInventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Inventory Component should have PlayerController as Owner."));
	if (!OwningController->IsLocalController()) return;

	InventoryMenu = CreateWidget<UInventoryBase>(OwningController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	CloseInventoryMenu();
}

void UInventoryComponent::TryAddItem(UItemComponent* ItemComponent)
{
	FSlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

	UInventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());
	Result.Item = FoundItem;
	
	if (Result.TotalRoomToFill == 0)
	{
		NoRoomInInventory.Broadcast();
		return;
	}

	if (Result.Item.IsValid() && Result.bStackable)
	{
		// Add Stacks to existing Item
		OnStackChange.Broadcast(Result);
		Server_AddStacksToItem(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0)
	{
		// Create new item
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0, Result.Remainder);
	}
}

void UInventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

void UInventoryComponent::Server_AddNewItem_Implementation(UItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	UInventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);
	NewItem->SetTotalStackCount(StackCount);

	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAdded.Broadcast(NewItem);
	}

	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FStackableFragment* StackableFragment = ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FStackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void UInventoryComponent::Server_AddStacksToItem_Implementation(UItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	const FGameplayTag& ItemType = IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemType() : FGameplayTag::EmptyTag;
	UInventoryItem* Item = InventoryList.FindFirstItemByType(ItemType);
	
	if (!IsValid(Item)) return;

	Item->SetTotalStackCount(Item->GetTotalStackCount() + StackCount);

	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FStackableFragment* StackableFragment = ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FStackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void UInventoryComponent::Server_DropItem_Implementation(UInventoryItem* Item, int32 StackCount)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - StackCount;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}
	SpawnDroppedItem(Item, StackCount);
}

void UInventoryComponent::SpawnDroppedItem(UInventoryItem* Item, int32 StackCount)
{
	const APawn* OwningPawn = OwningController->GetPawn();
	FVector RotatedForward = OwningPawn->GetActorForwardVector();
	RotatedForward = RotatedForward.RotateAngleAxis(FMath::FRandRange(DropSpawnAngleMin, DropSpawnAngleMax), FVector::UpVector);
	FVector SpawnLocation = OwningPawn->GetActorLocation() + RotatedForward * FMath::FRandRange(DropSpawnDistanceMin, DropSpawnDistanceMax);
	SpawnLocation.Z -= RelativeSpawnElevation;
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	FItemManifest& ItemManifest = Item->GetItemManifestMutable();
	if (FStackableFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FStackableFragment>())
	{
		StackableFragment->SetStackCount(StackCount);
	}
	ItemManifest.SpawnPickupActor(this, SpawnLocation, SpawnRotation);
}

void UInventoryComponent::Server_ConsumeItem_Implementation(UInventoryItem* Item)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - 1;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	if (FConsumableFragment* ConsumableFragment = Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FConsumableFragment>())
	{
		ConsumableFragment->OnConsume(OwningController.Get());
	}
}

void UInventoryComponent::Server_ConsumeItemOfTypAndAmount_Implementation(const FGameplayTag& ItemType, const int32 Amount)
{
	UInventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemType);
	
	const int32 NewStackCount = FoundItem->GetTotalStackCount() - Amount;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(FoundItem);
	}
	else
	{
		FoundItem->SetTotalStackCount(NewStackCount);
	}
	
	OnItemStackChange.Broadcast(ItemType, Amount);

	if (FConsumableFragment* ConsumableFragment = FoundItem->GetItemManifestMutable().GetFragmentOfTypeMutable<FConsumableFragment>())
	{
		ConsumableFragment->OnConsume(OwningController.Get());
	}
}

bool UInventoryComponent::CheckItemOfTypAndAmount(const FGameplayTag& ItemType,  const int32 Amount)
{
	UInventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemType);
	bool bHasItemWithAmount = IsValid(FoundItem) && FoundItem->GetTotalStackCount() >= Amount;
	if (!bHasItemWithAmount) NoItemOfTypeInInventory.Broadcast(ItemType.GetTagLeafName());
	return bHasItemWithAmount;
}

void UInventoryComponent::Server_EquippedSlottedItemClicked_Implementation(UInventoryItem* ItemToEquip, UInventoryItem* ItemToUnequip)
{
	Multicast_EquippedSlottedItemClicked(ItemToEquip, ItemToUnequip);
}

void UInventoryComponent::Multicast_EquippedSlottedItemClicked_Implementation(UInventoryItem* ItemToEquip, UInventoryItem* ItemToUnequip)
{
	// EquipmentComponent listen to Equip/Unequip delegates
	OnItemEquipped.Broadcast(ItemToEquip);
	OnItemUnequipped.Broadcast(ItemToUnequip);
}

void UInventoryComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
	OnInventoryMenuToggled.Broadcast(bInventoryMenuOpen);
}

void UInventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UInventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
}

