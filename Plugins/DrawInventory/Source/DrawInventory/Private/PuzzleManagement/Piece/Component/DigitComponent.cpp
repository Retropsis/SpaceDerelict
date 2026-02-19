// Retropsis 2026

#include "PuzzleManagement/Piece/Component/DigitComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Widget/Puzzle/DigitWidget.h"

UDigitComponent::UDigitComponent()
{
}

UWidgetComponent* UDigitComponent::SetupDigitWidgetComponent()
{
	// DigitWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("DigitWidgetComponent");
	// DigitWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	// DigitWidgetComponent->SetDrawAtDesiredSize(true);
	// DigitWidgetComponent->SetDrawSize(FVector2D( 16.f, 16.f ));
	//
	// return DigitWidgetComponent;
	return nullptr;
}

void UDigitComponent::BeginPlay()
{
	Super::BeginPlay();
	// DigitWidgetComponent->SetRelativeLocation(FVector::ZeroVector);
}

void UDigitComponent::PressInput() const
{
	OnNumericInput.Broadcast(DigitCode.GetTagLeafName());
}

void UDigitComponent::SetDigitCode(const FGameplayTag& Digit)
{
	DigitCode = Digit;
	if (IsValid(DigitWidget)) DigitWidget->SetDigit(Digit.ToString());
}

