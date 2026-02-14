// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Inventory/GridSlot/InventoryGridSlot.h"
#include "Item/InventoryItem.h"
#include "Widget/ItemPopUp/ItemPopUp.h"
#include "Components/Image.h"

void UInventoryGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	GridSlotHovered.Broadcast(TileIndex, InMouseEvent);
}

void UInventoryGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	GridSlotUnhovered.Broadcast(TileIndex, InMouseEvent);
}

FReply UInventoryGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	GridSlotClicked.Broadcast(TileIndex, InMouseEvent);
	return FReply::Handled();
}

void UInventoryGridSlot::SetInventoryItem(UInventoryItem* Item)
{
	InventoryItem = Item;
}

void UInventoryGridSlot::SetItemPopUp(UItemPopUp* PopUp)
{
	ItemPopUp = PopUp;
	ItemPopUp->SetGridIndex(GetTileIndex());
	ItemPopUp->OnNativeDestruct.AddUObject(this, &ThisClass::OnItemPopUpDestruct);
}

void UInventoryGridSlot::OnItemPopUpDestruct(UUserWidget* Menu)
{
	ItemPopUp.Reset();
}

UItemPopUp* UInventoryGridSlot::GetItemPopUp() const
{
	return ItemPopUp.Get(); 
}

void UInventoryGridSlot::SetUnoccupiedTexture()
{
	GridSlotState = EGridSlotState::Unoccupied;
	Image_GridSlot->SetBrush(Brush_Unoccupied);
}

void UInventoryGridSlot::SetOccupiedTexture()
{
	GridSlotState = EGridSlotState::Occupied;
	Image_GridSlot->SetBrush(Brush_Occupied);
}

void UInventoryGridSlot::SetSelectedTexture()
{
	GridSlotState = EGridSlotState::Selected;
	Image_GridSlot->SetBrush(Brush_Selected);
}

void UInventoryGridSlot::SetGrayedOutTexture()
{
	GridSlotState = EGridSlotState::GrayedOut;
	Image_GridSlot->SetBrush(Brush_GrayedOut);
}
