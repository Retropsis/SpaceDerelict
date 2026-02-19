// Retropsis 2026

#include "PuzzleManagement/Piece/SafeBox.h"

#include "Components/WidgetComponent.h"
#include "PuzzleManagement/Piece/Component/DigitComponent.h"
#include "PuzzleManagement/PuzzleTags.h"
#include "Widget/Puzzle/DigitWidget.h"

#define CREATE_AND_SETUP_DIGIT(Digit_X) \
Digit_X = CreateDefaultSubobject<UDigitComponent>(TEXT(#Digit_X)); \
Digits.Add(Digit_X); \
Digit_X->SetupAttachment(GetRootComponent());

ASafeBox::ASafeBox()
{
	PrimaryActorTick.bCanEverTick = false;
	
	NumericPadRoot = CreateDefaultSubobject<USceneComponent>("NumericPadRoot");
	SetRootComponent(NumericPadRoot);
	
	CREATE_AND_SETUP_DIGIT(Digit_1);
	Digit_1->SetDigitCode(Puzzle::Numeric::One);
	UWidgetComponent* DigitComp_1 = ConstructDigitWidgetComponent(FName("DigitWidgetComponent_1"));
	DigitComp_1->SetupAttachment(Digit_1);
	
	CREATE_AND_SETUP_DIGIT(Digit_2);
	Digit_1->SetDigitCode(Puzzle::Numeric::Two);
	UWidgetComponent* DigitComp_2 = ConstructDigitWidgetComponent(FName("DigitWidgetComponent_2"));
	DigitComp_2->SetupAttachment(Digit_2);
	
	CREATE_AND_SETUP_DIGIT(Digit_3);
	Digit_1->SetDigitCode(Puzzle::Numeric::Three);
	UWidgetComponent* DigitComp_3 = ConstructDigitWidgetComponent(FName("DigitWidgetComponent_3"));
	DigitComp_3->SetupAttachment(Digit_3);
	
	CREATE_AND_SETUP_DIGIT(Digit_4);
	Digit_1->SetDigitCode(Puzzle::Numeric::Four);
	UWidgetComponent* DigitComp_4 = ConstructDigitWidgetComponent(FName("DigitWidgetComponent_4"));
	DigitComp_4->SetupAttachment(Digit_4);
	
	CREATE_AND_SETUP_DIGIT(Digit_5);
	Digit_1->SetDigitCode(Puzzle::Numeric::Five);
	UWidgetComponent* DigitComp_5 = ConstructDigitWidgetComponent(FName("DigitWidgetComponent_5"));
	DigitComp_5->SetupAttachment(Digit_5);
	
	CREATE_AND_SETUP_DIGIT(Digit_6);
	Digit_1->SetDigitCode(Puzzle::Numeric::Six);
	UWidgetComponent* DigitComp_6 = ConstructDigitWidgetComponent(FName("DigitWidgetComponent_6"));
	DigitComp_6->SetupAttachment(Digit_6);
	
	CREATE_AND_SETUP_DIGIT(Digit_7);
	Digit_1->SetDigitCode(Puzzle::Numeric::Seven);
	UWidgetComponent* DigitComp_7 = ConstructDigitWidgetComponent(FName("DigitWidgetComponent_7"));
	DigitComp_7->SetupAttachment(Digit_7);
	
	CREATE_AND_SETUP_DIGIT(Digit_8);
	Digit_1->SetDigitCode(Puzzle::Numeric::Eight);
	UWidgetComponent* DigitComp_8 = ConstructDigitWidgetComponent(FName("DigitWidgetComponent_8"));
	DigitComp_8->SetupAttachment(Digit_8);
	
	CREATE_AND_SETUP_DIGIT(Digit_9);
	Digit_1->SetDigitCode(Puzzle::Numeric::Nine);
	UWidgetComponent* DigitComp_9 = ConstructDigitWidgetComponent(FName("DigitWidgetComponent_9"));
	DigitComp_9->SetupAttachment(Digit_9);
	
	CREATE_AND_SETUP_DIGIT(Digit_0);
	Digit_1->SetDigitCode(Puzzle::Numeric::Zero);
	UWidgetComponent* DigitComp_0 = ConstructDigitWidgetComponent(FName("DigitWidgetComponent_0"));
	DigitComp_0->SetupAttachment(Digit_0);
	
	CREATE_AND_SETUP_DIGIT(Digit_Enter);
	Digit_Enter->SetDigitCode(Puzzle::Numeric::Enter);
	UWidgetComponent* DigitComp_Enter = ConstructDigitWidgetComponent(FName("DigitWidgetComponent_Enter"));
	DigitComp_Enter->SetupAttachment(Digit_Enter);

	for (const TObjectPtr<UDigitComponent> Digit : Digits)
	{
		Digit->OnNumericInput.AddDynamic(this, &ThisClass::OnNumericInput);
	}
	
	DigitWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("DigitWidgetComponent");
	DigitWidgetComponent->SetupAttachment(GetRootComponent());
	DigitWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	DigitWidgetComponent->SetDrawAtDesiredSize(true);
	DigitWidgetComponent->SetDrawSize(FVector2D( 200.f, 200.f ));
}

void ASafeBox::OnNumericInput(const FName& Digit)
{
	if (bUnlocked) return;
	if (Digit == FName("Enter")) ResetInputCode();
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Digit.ToString());
	const int32 NewDigit = FCString::Atoi(*Digit.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Conversion to %d"), NewDigit);
	InputCode.Add(NewDigit);
	
	if (IsValid(DigitWidget))
	{
		FString NewDigitCode;
		for (const uint32 Number : InputCode)
		{
			NewDigitCode.Append(FString::FromInt(Number));
		}
		DigitWidget->SetDigit(NewDigitCode);
	}

	bool bCodeCorrect = true;
	for (int i = 0; i < SafeBoxCode.Num(); ++i)
	{
		if (InputCode.IsValidIndex(i) && InputCode[i] == SafeBoxCode[i]) continue;
		bCodeCorrect = false;
	}
	if (bCodeCorrect)
	{
		Unlock();
		bUnlocked = true;
	}
	else ResetInputCode();
}

void ASafeBox::BeginPlay()
{
	Super::BeginPlay();
	DigitWidget = Cast<UDigitWidget>(DigitWidgetComponent->GetWidget());
}

void ASafeBox::ResetInputCode()
{
	if (InputCode.Num() == SafeBoxCode.Num())
	{
		InputCode.Empty();
		if (IsValid(DigitWidget)) DigitWidget->SetDigit(FString());
	}
}

UWidgetComponent* ASafeBox::ConstructDigitWidgetComponent(const FName& Name)
{
	UWidgetComponent* NewWidget = CreateDefaultSubobject<UWidgetComponent>(Name);
	NewWidget->SetWidgetSpace(EWidgetSpace::World);
	NewWidget->SetDrawAtDesiredSize(true);
	NewWidget->SetDrawSize(FVector2D( 16.f, 16.f ));
	NewWidget->SetWidget(DigitWidget);
	return NewWidget;
}

