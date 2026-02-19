// Retropsis 2026

#include "Widget/Puzzle/DigitWidget.h"
#include "Components/TextBlock.h"

void UDigitWidget::SetDigit(const FString& NewDigit) const
{
	Text_Digit->SetText(FText::FromString(NewDigit));
}
