// Retropsis 2026


#include "Widget/DrawInventory/SlottedRoom/SlottedRoom.h"
#include "Components/Image.h"
#include "Item/InventoryItem.h"

void USlottedRoom::SetInventoryItem(UInventoryItem* Item)
{
	InventoryItem = Item;
}

void USlottedRoom::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void USlottedRoom::SetImageTransformAngle(float Angle) const
{
	Image_Icon->SetRenderTransformAngle(Angle);
}
