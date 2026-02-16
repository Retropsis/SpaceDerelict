// Retropsis 2026

#include "Widget/DrawInventory/SlottedRoom/DrawnRoomSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Item/InventoryItem.h"

FReply UDrawnRoomSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnDrawnRoomSlotClicked.Broadcast(this);
	return FReply::Handled();
}

void UDrawnRoomSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	OnDrawnRoomSlotHovered.Broadcast(this);
}

void UDrawnRoomSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnDrawnRoomSlotUnhovered.Broadcast(this);
}

void UDrawnRoomSlot::SetRequirement(UTexture2D* Icon, const int32 Amount, bool bRequirementMet) const
{
	HorizontalBox_Requirement->SetVisibility(ESlateVisibility::Visible);

	UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	Image->SetBrushResourceObject(Icon);
	FSlateBrush Brush;
	Brush.DrawAs = ESlateBrushDrawType::Image;

	HorizontalBox_Requirement->SetRenderTranslation(FVector2D(0.f, /*Image_Icon->GetCachedGeometry().GetLocalSize().Y*/ 160.f));
	
	for (int i = 0; i < Amount; ++i)
	{
		HorizontalBox_Requirement->AddChildToHorizontalBox(Image);
	}
	
	UHorizontalBoxSlot* ImageSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(Image);
	FSlateChildSize Size;
	Size.SizeRule = ESlateSizeRule::Automatic;
	ImageSlot->SetSize(Size);
	ImageSlot->SetPadding(FMargin(5.f, 0.f, 5.f, 0.f));
	
	Text_Requirement->SetColorAndOpacity(bRequirementMet ? RequirementMetColor : RequirementNotMatchedColor);
	Image->SetColorAndOpacity(bRequirementMet ? RequirementMetColor : RequirementNotMatchedColor);
}

void UDrawnRoomSlot::SetValuable(const TMap<UTexture2D*, int32>& Valuables) const
{
	HorizontalBox_Valuable->SetVisibility(ESlateVisibility::Visible);

	TArray<TTuple<UTexture2D*, int>> ValuablesArray = Valuables.Array();
	for (int i = 0; i < ValuablesArray.Num(); ++i)
	{
		UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		Image->SetBrushResourceObject(ValuablesArray[i].Key);
		FSlateBrush Brush;
		Brush.DrawAs = ESlateBrushDrawType::Image;
		HorizontalBox_Valuable->AddChildToHorizontalBox(Image);
		
		UHorizontalBoxSlot* ImageSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(Image);
		FSlateChildSize Size;
		Size.SizeRule = ESlateSizeRule::Automatic;
		ImageSlot->SetSize(Size);
		ImageSlot->SetPadding(FMargin(5.f, 0.f, 5.f, 0.f));
		
		UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		if (i < ValuablesArray.Num() - 1) Text->SetText(FText::FromString(","));
		HorizontalBox_Valuable->AddChildToHorizontalBox(Text);
	}
}

void UDrawnRoomSlot::SetInventoryItem(UInventoryItem* Item)
{
	InventoryItem = Item;
}

void UDrawnRoomSlot::SetSizeBox(float TileSize) const
{
	SizeBox_DrawRoomSlot->SetWidthOverride(TileSize);
	SizeBox_DrawRoomSlot->SetHeightOverride(TileSize);
}

void UDrawnRoomSlot::SetRoomIcon(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void UDrawnRoomSlot::SetRoomName(const FText& Name) const
{
	Text_RoomName->SetText(Name);
}
