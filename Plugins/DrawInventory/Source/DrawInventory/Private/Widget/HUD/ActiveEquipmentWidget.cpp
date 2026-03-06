// Retropsis 2026

#include "Widget/HUD/ActiveEquipmentWidget.h"
#include "Components/Image.h"

void UActiveEquipmentWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetIcon(true);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UActiveEquipmentWidget::SetIcon(bool bActivated) const
{
	Image_Equipment->SetBrush(bActivated ? Brush_Activated : Brush_Deactivated);
}
