// Retropsis 2026

#include "Widget/Knowledge/SymbolWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USymbolWidget::SetImage(UTexture2D* Texture)
{
	Image_Symbol->SetBrushResourceObject(Texture);	
}

void USymbolWidget::SetText(FText Text)
{
	Text_Symbol->SetText(Text);
}
