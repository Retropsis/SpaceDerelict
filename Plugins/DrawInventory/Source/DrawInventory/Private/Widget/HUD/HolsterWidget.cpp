// Retropsis 2026

#include "Widget/HUD/HolsterWidget.h"
#include "Components/Image.h"

void UHolsterWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetIcon(true);
}

void UHolsterWidget::SetIcon(bool bEquipped) const
{
	FSlateBrush Brush;
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.SetImageSize(FVector2D{IconSize});
	Brush.SetResourceObject(bEquipped ? Icon_Equipped : Icon_Unequipped);
	Image_Holster->SetBrush(Brush);
}
