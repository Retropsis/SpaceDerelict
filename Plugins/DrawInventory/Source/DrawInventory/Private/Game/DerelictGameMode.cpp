// Retropsis 2026

#include "Game/DerelictGameMode.h"
#include "Data/PuzzleData.h"

void ADerelictGameMode::BeginPlay()
{
	Super::BeginPlay();
	ConstructPuzzleMeta();
}

void ADerelictGameMode::ConstructPuzzleMeta()
{
	checkf(IsValid(PuzzleInfo), TEXT("Puzzle Info must be filled in Derelict Game Mode"));

	PuzzleInfo->ConstructPuzzleMeta();
	for (const TInstancedStruct<FPuzzlePattern>& PuzzlePattern : PuzzleInfo->GetPuzzlePatterns())
	{
		PuzzleData.Add(PuzzlePattern);

		UE_LOG(LogTemp, Display, TEXT("Puzzle Data:"));
	}
}

FString ADerelictGameMode::GetHintMessageWithTag(const FGameplayTag& Tag) const
{
	for (const TInstancedStruct<FPuzzlePattern>& Data : PuzzleData)
	{
		if (const FPuzzlePattern* DataPtr = Data.GetPtr<FPuzzlePattern>())
		{
			if (!DataPtr->GetTag().MatchesTagExact(Tag)) continue;
			return DataPtr->GetHintMessage();
		}
	}
	return FString();
}
