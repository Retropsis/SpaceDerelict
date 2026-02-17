// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUD/HUDWidget.h"
#include "Blueprint/WidgetTree.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "InventoryManagement/Utilities/InventoryUtility.h"
#include "Widget/DrawInventory/HUD/HUDCounter.h"
#include "Widget/HUD/InfoMessage.h"

void UHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UInventoryComponent* InventoryComponent = UInventoryUtility::GetInventoryComponent(GetOwningPlayer());
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->NoRoomInInventory.AddDynamic(this, &ThisClass::OnNoRoom);
		InventoryComponent->NoItemOfTypeInInventory.AddDynamic(this, &ThisClass::OnNoItemOfTypeAndAmount);
		InventoryComponent->OnHUDCounterItemStackChange.AddDynamic(this, &ThisClass::UpdateHUDCounter);
	}
	InitializeHUDCounters();
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
