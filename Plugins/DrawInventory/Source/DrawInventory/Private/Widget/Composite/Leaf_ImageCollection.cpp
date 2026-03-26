// Retropsis 2026

#include "Widget/Composite/Leaf_ImageCollection.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"

void ULeaf_ImageCollection::SetCollectionVisibility(bool bCollapse) const
{
	HorizontalBox_Collection->SetVisibility(bCollapse ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void ULeaf_ImageCollection::AddImageToCollection(UTexture2D* Icon) const
{
	UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	FSlateBrush Brush;
	Brush.SetResourceObject(Icon);
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = FVector2D(64.f, 64.f);
	Image->SetBrush(Brush);
	HorizontalBox_Collection->AddChildToHorizontalBox(Image);
		
	UHorizontalBoxSlot* ImageSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(Image);
	FSlateChildSize Size;
	Size.Value = 256.f;
	Size.SizeRule = ESlateSizeRule::Automatic;
	ImageSlot->SetHorizontalAlignment(HAlign_Center);
	ImageSlot->SetSize(Size);
	ImageSlot->SetPadding(FMargin(5.f, 0.f, 5.f, 0.f));	
}

void ULeaf_ImageCollection::ClearImageCollection() const
{
	HorizontalBox_Collection->ClearChildren();
}
