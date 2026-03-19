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
	// SetRootComponent(NumericPadRoot);
	
	CREATE_AND_SETUP_DIGIT(Digit_1);
	Digit_1->SetDigitCode(Puzzle::Numeric::One);
	
	CREATE_AND_SETUP_DIGIT(Digit_2);
	Digit_1->SetDigitCode(Puzzle::Numeric::Two);
	
	CREATE_AND_SETUP_DIGIT(Digit_3);
	Digit_1->SetDigitCode(Puzzle::Numeric::Three);
	
	CREATE_AND_SETUP_DIGIT(Digit_4);
	Digit_1->SetDigitCode(Puzzle::Numeric::Four);
	
	CREATE_AND_SETUP_DIGIT(Digit_5);
	Digit_1->SetDigitCode(Puzzle::Numeric::Five);
	
	CREATE_AND_SETUP_DIGIT(Digit_6);
	Digit_1->SetDigitCode(Puzzle::Numeric::Six);
	
	CREATE_AND_SETUP_DIGIT(Digit_7);
	Digit_1->SetDigitCode(Puzzle::Numeric::Seven);
	
	CREATE_AND_SETUP_DIGIT(Digit_8);
	Digit_1->SetDigitCode(Puzzle::Numeric::Eight);
	
	CREATE_AND_SETUP_DIGIT(Digit_9);
	Digit_1->SetDigitCode(Puzzle::Numeric::Nine);
	
	CREATE_AND_SETUP_DIGIT(Digit_0);
	Digit_1->SetDigitCode(Puzzle::Numeric::Zero);
	
	CREATE_AND_SETUP_DIGIT(Digit_Enter);
	Digit_Enter->SetDigitCode(Puzzle::Numeric::Enter);

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

void ASafeBox::SetSafeBoxCode(const FString& Code)
{
	for (const TCHAR Char : Code.GetCharArray())
	{
		if (FChar::IsDigit(Char))
		{
			UE_LOG(LogTemp, Warning, TEXT("Converting %c to %d"), Char, FChar::ConvertCharDigitToInt(Char))
			SafeBoxCode.Add(FChar::ConvertCharDigitToInt(Char));
		}
	}
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
		SpawnReward();
		bUnlocked = true;
	}
	else ResetInputCode();
}

void ASafeBox::SpawnReward() const
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (IsValid(LootItemClass))
	{
		GetWorld()->SpawnActor<AActor>(LootItemClass, GetSpawnTransform(), SpawnParams);
	}
}

void ASafeBox::BeginPlay()
{
	Super::BeginPlay();
	DigitWidget = Cast<UDigitWidget>(DigitWidgetComponent->GetWidget());

	if (!IsValid(DigitWidget)) return;
	
	TArray<UWidgetComponent*> Widgets;
	GetComponents(UWidgetComponent::StaticClass(), Widgets);

	for (UWidgetComponent* Widget : Widgets)
	{
		Widget->SetWidget(DigitWidget);
	}
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
	return NewWidget;
}

