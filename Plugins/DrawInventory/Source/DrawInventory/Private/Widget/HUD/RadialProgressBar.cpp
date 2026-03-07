// Retropsis 2026

#include "Widget/HUD/RadialProgressBar.h"
#include "Components/Image.h"

void URadialProgressBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ConstructDynamicMaterial();
	SetVisibility(ESlateVisibility::Collapsed);
}

void URadialProgressBar::ConstructDynamicMaterial()
{
	if (!IsValid(MID_Radial))
	{
		MID_Radial = UMaterialInstanceDynamic::Create(RadialMaterial, this);
		Image_Radial->SetBrushResourceObject(MID_Radial);
	}
}

void URadialProgressBar::SetPercentage(float Percentage)
{
	ConstructDynamicMaterial();
	MID_Radial->SetScalarParameterValue(FName("Percentage"), Percentage);

	if (Percentage > 0.f && Percentage <= 1.f)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}
