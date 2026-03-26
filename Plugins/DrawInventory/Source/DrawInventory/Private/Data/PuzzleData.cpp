// Retropsis 2026

#include "Data/PuzzleData.h"
#include "PuzzleManagement/PuzzleTags.h"

void UPuzzleData::ConstructPuzzleMeta()
{
	const int32 TrueBoxSelection = FMath::RandRange(0, TrueBoxPatterns.Num() - 1);
	PuzzlePatterns.Add(TrueBoxPatterns[TrueBoxSelection]);
	
	const int32 SafeBoxSelection = FMath::RandRange(0, SafeBoxPatterns.Num() - 1);
	PuzzlePatterns.Add(SafeBoxPatterns[SafeBoxSelection]);
	
	const int32 DeliverySelection = FMath::RandRange(0,DeliveryPatterns.Num() - 1);
	PuzzlePatterns.Add(DeliveryPatterns[DeliverySelection]);
	
	const int32 HieroglyphicCombinationSelection = FMath::RandRange(0, HieroglyphicCombinationPatterns.Num() - 1);
	PuzzlePatterns.Add(HieroglyphicCombinationPatterns[HieroglyphicCombinationSelection]);
	
	const int32 BreakableSelection = FMath::RandRange(0, BreakablePatterns.Num() - 1);
	PuzzlePatterns.Add(BreakablePatterns[BreakableSelection]);
}

FString FTrueBoxPattern::GetHintMessage() const
{
	return FPuzzlePattern::GetHintMessage();
}

FString FSafeBoxPattern::GetHintMessage() const
{
	return FString::Printf(TEXT("Hello! The new code is %s! For what? I don't know!"), *Code);
}

FString FDeliveryPattern::GetHintMessage() const
{
	return FPuzzlePattern::GetHintMessage();
}

FString FHieroglyphicCombinationPattern::GetHintMessage() const
{
	// FString Message = FString();
	// for (const FGameplayTag& Hieroglyphic : HieroglyphicCombination)
	// {
	// 	Message.Append(FString::Printf(TEXT("%s, "), *Hieroglyphic.GetTagLeafName().ToString()));
	// }
	// return Message;
	return FString::Printf(TEXT("Symbols are carved on this object, it seems to be phoenician."));
}

FString FBreakablePattern::GetHintMessage() const
{
	return FPuzzlePattern::GetHintMessage();
}
