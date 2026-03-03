// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUD/HUDWidget.h"
#include "Blueprint/WidgetTree.h"
#include "EquipmentManagement/Component/EquipmentComponent.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "InventoryManagement/Utilities/InventoryUtility.h"
#include "Item/ItemTags.h"
#include "Widget/DrawInventory/HUD/HUDCounter.h"
#include "Widget/HUD/HolsterWidget.h"
#include "Widget/HUD/InfoMessage.h"


void UHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UInventoryComponent* InventoryComponent = UInventoryUtility::GetInventoryComponent(GetOwningPlayer());
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->NoRoomInInventory.AddDynamic(this, &ThisClass::OnNoRoom);
		InventoryComponent->NoItemOfTypeInInventory.AddDynamic(this, &ThisClass::OnNoItemOfTypeAndAmount);
		InventoryComponent->OnItemOfTypeStackChange.AddDynamic(this, &ThisClass::UpdateHUDCounter);
	}
	UEquipmentComponent* EquipmentComponent = UInventoryUtility::GetEquipmentComponent(GetOwningPlayer());
	if (IsValid(EquipmentComponent))
	{
		EquipmentComponent->OnEquipped.AddDynamic(this, &ThisClass::OnGunEquipped);
	}
	InitializeHUDCounters();
	InitializeHolsterWidget();
}

void UHUDWidget::InitializeHUDCounters()
{
	WidgetTree->ForEachWidget([this] (UWidget* Widget)
	{
		if (UHUDCounter* HUDCounter = Cast<UHUDCounter>(Widget); IsValid(HUDCounter))
		{
			HUDCounters.Add(HUDCounter->GetItemType(), HUDCounter);
		}
	});
}

void UHUDWidget::InitializeHolsterWidget()
{
	WidgetTree->ForEachWidget([this] (UWidget* Widget)
	{
		if (UHolsterWidget* FoundWidget = Cast<UHolsterWidget>(Widget); IsValid(FoundWidget))
		{
			HolsterWidget = FoundWidget;
		}
	});
}

void UHUDWidget::ToggleHUD(bool bShow)
{
	SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UHUDWidget::UpdateHUDCounter(const FGameplayTag& ItemType, int32 NewCount)
{
	if (HUDCounters.Contains(ItemType))
	{
		HUDCounters[ItemType]->SetCount(NewCount);
	}
}

void UHUDWidget::UpdateHolsterWidget(bool bEquipped, bool bVisible)
{
	if (IsValid(HolsterWidget))
	{
		HolsterWidget->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		HolsterWidget->SetIcon(bEquipped);
	}
}

void UHUDWidget::OnGunEquipped(const FGameplayTag& EquipmentType)
{
	if (EquipmentType.MatchesTagExact(Item::Equipment::Weapons::Gun))
	{
		UpdateHolsterWidget(true, true);
	}
}

void UHUDWidget::OnGunUnequipped(const FGameplayTag& EquipmentType)
{
	if (EquipmentType.MatchesTagExact(Item::Equipment::Weapons::Gun))
	{
		UpdateHolsterWidget(false, false);
	}
}

void UHUDWidget::OnNoRoom()
{
	if (!IsValid(InfoMessage)) return;

	InfoMessage->SetMessage(FText::FromString("No Room In Inventory."));
}

void UHUDWidget::OnNoItemOfTypeAndAmount(const FName& ItemName)
{
	if (!IsValid(InfoMessage)) return;
	const FString Message = FString::Printf(TEXT("No %s In Inventory."), *ItemName.ToString());
	InfoMessage->SetMessage(FText::FromString(Message));
}
