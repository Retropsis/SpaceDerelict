// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUD/HUDWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "EquipmentManagement/Component/EquipmentComponent.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "InventoryManagement/Utilities/InventoryUtility.h"
#include "Item/ItemTags.h"
#include "Widget/DrawInventory/HUD/HUDCounter.h"
#include "Widget/HUD/ActiveEquipmentWidget.h"
#include "Widget/HUD/HelpPage.h"
#include "Widget/HUD/InfoMessage.h"
#include "Widget/HUD/RadialProgressBar.h"
#include "Widget/Knowledge/KnowledgeLog.h"
#include "Widget/MainMenu/MainMenu.h"


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
	InitializeMenus();
}

void UHUDWidget::InitializeMenus()
{
	WidgetTree->ForEachWidget([this] (UWidget* Widget)
	{
		if (UKnowledgeLog* KnowledgeLogWidget = Cast<UKnowledgeLog>(Widget); IsValid(KnowledgeLogWidget))
		{
			KnowledgeLog = KnowledgeLogWidget;
			KnowledgeLog->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (UMainMenu* MainMenuWidget = Cast<UMainMenu>(Widget); IsValid(MainMenuWidget))
		{
			MainMenu = MainMenuWidget;
			MainMenu->SetVisibility(ESlateVisibility::Collapsed);
			MainMenu->ResumeButton->OnClicked.AddDynamic(this, &ThisClass::CloseMainMenu);
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

void UHUDWidget::CloseMainMenu()
{
	if (!IsValid(MainMenu)) return;

	MainMenu->SetVisibility(ESlateVisibility::Collapsed);
	bMainMenuOpen = false;

	SetInputModeGameOnly();
}

void UHUDWidget::OpenMainMenu()
{
	if (!IsValid(MainMenu)) return;

	MainMenu->SetVisibility(ESlateVisibility::Visible);
	bMainMenuOpen = true;

	SetInputModeGameAndUI();
}

void UHUDWidget::ToggleMainMenu()
{
	if (bMainMenuOpen)
	{
		CloseMainMenu();
	}
	else
	{
		OpenMainMenu();
	}
}

void UHUDWidget::ToggleHelpPage()
{
	if (bHelpPageOpen)
	{
		HelpPage->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		HelpPage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	bHelpPageOpen = !bHelpPageOpen;
}

void UHUDWidget::CloseKnowledgeLog()
{
	if (!IsValid(KnowledgeLog)) return;

	KnowledgeLog->SetVisibility(ESlateVisibility::Collapsed);
	bKnowledgeLogOpen = false;

	SetInputModeGameOnly();
}

void UHUDWidget::OpenKnowledgeLog()
{
	if (!IsValid(KnowledgeLog)) return;

	KnowledgeLog->SetVisibility(ESlateVisibility::Visible);
	bKnowledgeLogOpen = true;

	SetInputModeGameAndUI();
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
			if (Widget->GetEquipmentType().MatchesTagExact(EquipmentType))
			{
				if (bNewlyEquipped)
				{
					Widget->SetVisibility(ESlateVisibility::HitTestInvisible);
				}
				Widget->OnEquipVisualEffects();
				Widget->SetIcon(true);
			}
			else
			{
				Widget->SetIcon(false);
				Widget->OnAimDownSightVisualEffects(false);
			}
		}
	}
}

void UHUDWidget::UpdateActiveEquipmentADS(const FGameplayTag& EquipmentType, bool bAimDownSight)
{
	for (TObjectPtr<UActiveEquipmentWidget> Widget : ActiveEquipmentWidgets)
	{
		if (IsValid(Widget))
		{
			if (Widget->GetEquipmentType().MatchesTagExact(EquipmentType))
			{
				Widget->OnAimDownSightVisualEffects(bAimDownSight);
			}
		}
	}
}

void UHUDWidget::OnActiveEquipped(const FGameplayTag& EquipmentType)
{
	UpdateActiveEquipmentWidget(EquipmentType, true);
}

void UHUDWidget::OnScanStarted()
{
	if (IsValid(RadialProgressBar))
	{
		RadialProgressBar->OnStartVisualEffect();
	}
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
		RadialProgressBar->OnCompletedVisualEffect();
		FTimerHandle CollapseTimer;
		GetWorld()->GetTimerManager().SetTimer(CollapseTimer, [this] ()
		{ 
			RadialProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		}, .2f, false);
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

void UHUDWidget::SetInputModeGameOnly() const
{
	if (!IsValid(GetOwningPlayer())) return;
	const FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);
}

void UHUDWidget::SetInputModeGameAndUI() const
{
	if (!IsValid(GetOwningPlayer())) return;
	const FInputModeGameAndUI InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
}
