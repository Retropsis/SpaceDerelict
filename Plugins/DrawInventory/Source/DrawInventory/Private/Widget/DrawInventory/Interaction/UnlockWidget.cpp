// Retropsis 2026

#include "Widget/DrawInventory/Interaction/UnlockWidget.h"
#include "Components/Button.h"

void UUnlockWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Unlock->OnClicked.AddDynamic(this, &ThisClass::OnUnlockButtonClicked);
	Button_Cancel->OnClicked.AddDynamic(this, &ThisClass::OnCancelButtonClicked);
}

void UUnlockWidget::OnUnlockButtonClicked()
{
	UnlockButtonClicked.Broadcast();
	RemoveFromParent();
}

void UUnlockWidget::OnCancelButtonClicked()
{
	RemoveFromParent();
}
