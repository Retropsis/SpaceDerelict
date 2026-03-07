// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUD/HUDWidget.h"
#include "Blueprint/WidgetTree.h"
#include "EquipmentManagement/Component/EquipmentComponent.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "InventoryManagement/Utilities/InventoryUtility.h"
#include "Item/ItemTags.h"
#include "Widget/DrawInventory/HUD/HUDCounter.h"
#include "Widget/HUD/ActiveEquipmentWidget.h"
#include "Widget/HUD/InfoMessage.h"
#include "Widget/HUD/RadialProgressBar.h"
#include "Widget/Knowledge/KnowledgeLog.h"


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
		EquipmentComponent->OnEquipped.AddDynamic(this, &ThisClass::OnActiveEquipped);
	}
	InitializeHUDCounters();
	InitializeActiveEquipmentWidget();
}

void UHUDWidget::InitializeKnowledgeLog()
{
	WidgetTree->ForEachWidget([this] (UWidget* Widget)
	{
		if (UKnowledgeLog* KnowledgeLogWidget = Cast<UKnowledgeLog>(Widget); IsValid(KnowledgeLogWidget))
		{
			KnowledgeLog = KnowledgeLogWidget;
			KnowledgeLog->SetVisibility(ESlateVisibility::Collapsed);
		}
	});
}

void UHUDWidget::ToggleKnowledgeLog()
{
	if (bKnowledgeLogOpen)
	{
		CloseKnowledgeLog();
	}
	else
	{
		OpenKnowledgeLog();
	}
}

void UHUDWidget::CloseKnowledgeLog()
{
	if (!IsValid(KnowledgeLog)) return;

	KnowledgeLog->SetVisibility(ESlateVisibility::Collapsed);
	bKnowledgeLogOpen = false;

	if (!IsValid(GetOwningPlayer())) return;

	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);
}

void UHUDWidget::OpenKnowledgeLog()
{
	if (!IsValid(KnowledgeLog)) return;

	KnowledgeLog->SetVisibility(ESlateVisibility::Visible);
	bKnowledgeLogOpen = true;

	if (!IsValid(GetOwningPlayer())) return;

	FInputModeGameAndUI InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
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

void UHUDWidget::InitializeActiveEquipmentWidget()
{
	WidgetTree->ForEachWidget([this] (UWidget* Widget)
	{
		if (UActiveEquipmentWidget* FoundWidget = Cast<UActiveEquipmentWidget>(Widget); IsValid(FoundWidget))
		{
			ActiveEquipmentWidgets.Add(FoundWidget);
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

void UHUDWidget::UpdateActiveEquipmentWidget(const FGameplayTag& EquipmentType, bool bNewlyEquipped)
{
	for (TObjectPtr<UActiveEquipmentWidget> Widget : ActiveEquipmentWidgets)
	{
		if (IsValid(Widget))
		{
			bool bMatchesTagExact = Widget->GetEquipmentType().MatchesTagExact(EquipmentType);
			if (bNewlyEquipped && bMatchesTagExact) Widget->SetVisibility(ESlateVisibility::HitTestInvisible);
			Widget->SetIcon(bMatchesTagExact);
		}
	}
}

void UHUDWidget::OnActiveEquipped(const FGameplayTag& EquipmentType)
{
	UpdateActiveEquipmentWidget(EquipmentType, true);
}

void UHUDWidget::OnScanProgress(float Percentage)
{
	if (IsValid(RadialProgressBar))
	{
		RadialProgressBar->SetPercentage(Percentage);
	}
}

void UHUDWidget::OnScanComplete(const UKnowledgeComponent* KnowledgeComponent)
{
	if (IsValid(KnowledgeLog))
	{
		KnowledgeLog->OnKnowledgeReceived(KnowledgeComponent);
	}
	if (IsValid(RadialProgressBar))
	{
		RadialProgressBar->SetVisibility(ESlateVisibility::Collapsed);
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
