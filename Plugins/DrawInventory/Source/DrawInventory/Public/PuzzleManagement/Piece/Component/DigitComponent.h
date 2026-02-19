// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "DigitComponent.generated.h"

class UDigitWidget;
class UWidgetComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNumericInput, const FName&, Digit);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UDigitComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UDigitComponent();
	void PressInput() const;
	FGameplayTag GetDigitCode() const { return DigitCode; }
	void SetDigitCode(const FGameplayTag& Digit);
	UWidgetComponent* SetupDigitWidgetComponent();
	
	FOnNumericInput OnNumericInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DrawInventory")
	UDigitWidget* DigitWidget;

protected:
virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TSubclassOf<UDigitWidget> DigitWidgetClass;
	
	// UPROPERTY(EditAnywhere, Category="DrawInventory")
	// TObjectPtr<UWidgetComponent> DigitWidgetComponent;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory", meta=(Categories="Puzzle.Numeric"))
	FGameplayTag DigitCode = FGameplayTag::EmptyTag;
};
