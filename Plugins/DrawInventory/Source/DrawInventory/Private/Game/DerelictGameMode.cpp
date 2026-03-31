// Retropsis 2026

#include "Game/DerelictGameMode.h"

#include "Data/KnowledgeData.h"
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

TArray<UTexture2D*> ADerelictGameMode::GetImageCollectionWithTag(const FGameplayTag& Tag) const
{
	for (const TInstancedStruct<FPuzzlePattern>& Data : PuzzleData)
	{
		if (const FHieroglyphicCombinationPattern* DataPtr = Data.GetPtr<FHieroglyphicCombinationPattern>())
		{
			if (!DataPtr->GetTag().MatchesTagExact(Tag)) continue;
			return ConstructImageCollectionFromTags(DataPtr->GetHieroglyphicCombination());
		}
	}
	return TArray<UTexture2D*>();
}

TArray<UTexture2D*> ADerelictGameMode::ConstructImageCollectionFromTags(const TArray<FGameplayTag>& SymbolTags) const
{
	TArray<UTexture2D*> Collection;
	for (const FGameplayTag& Tag : SymbolTags)
	{
		FSymbolData SymbolData = KnowledgeData->GetSymbolDataByTag(Tag);
		Collection.Add(SymbolData.Texture);
	}
	return Collection;
}

TMap<FIntPoint, UTexture2D*> ADerelictGameMode::ConstructDeliveryPanel(const FDeliveryPattern& DeliveryPattern)
{
	TMap<FIntPoint,  UTexture2D*> DeliveryPanel;
	for (const TTuple<FIntPoint, FPuzzleReward>& Pair : DeliveryPattern.GetRewardToCoordinates())
	{
		DeliveryPanel.Add(Pair.Key, Pair.Value.GetItemIcon());
	}
	return DeliveryPanel;
}
