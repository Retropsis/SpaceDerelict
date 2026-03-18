// Retropsis 2026


#include "Game/DerelictGameMode.h"

void ADerelictGameMode::AddPuzzleData(const TInstancedStruct<FPuzzleData>& Data)
{
	PuzzleData.Add(Data);
}

bool ADerelictGameMode::IsPuzzleDataValid(const FGameplayTag& PuzzleTag) const
{
	return PuzzleData.ContainsByPredicate([PuzzleTag] (const TInstancedStruct<FPuzzleData>& Data)
	{
		return PuzzleTag.MatchesTagExact(Data.GetPtr<>()->GetTag());
	});
}
