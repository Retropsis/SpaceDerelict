// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DigitWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API UDigitWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDigit(const FString& NewDigit) const;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Digit;
};
