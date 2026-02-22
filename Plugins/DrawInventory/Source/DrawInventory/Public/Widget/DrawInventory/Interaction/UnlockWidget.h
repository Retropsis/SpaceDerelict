// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnlockWidget.generated.h"

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnlockButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancelButtonClicked);

UCLASS()
class DRAWINVENTORY_API UUnlockWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	FOnUnlockButtonClicked UnlockButtonClicked;
	FOnCancelButtonClicked CancelButtonClicked;
	
private:
	UFUNCTION()
	void OnUnlockButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Unlock;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Cancel;
};
