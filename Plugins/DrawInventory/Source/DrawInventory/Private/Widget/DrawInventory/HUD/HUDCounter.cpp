// Retropsis 2026

#include "Widget/DrawInventory/HUD/HUDCounter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UHUDCounter::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetIcon(Icon);
	SetCount(0);
}

void UHUDCounter::SetIcon(UTexture2D* NewIcon) const
{
	FSlateBrush Brush;
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.SetImageSize(FVector2D{IconSize});
	Brush.SetResourceObject(NewIcon);
	Image_Icon->SetBrush(Brush);
}

void UHUDCounter::SetCount(int32 Count)
{
	if (!bAlwaysShow && Count <= 0)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
		OnNewCountVisualEffects();
	}
	Text_Count->SetText(FText::AsNumber(Count));
}
