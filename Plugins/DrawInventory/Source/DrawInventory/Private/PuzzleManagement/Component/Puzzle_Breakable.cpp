// Retropsis 2026

#include "PuzzleManagement/Component/Puzzle_Breakable.h"
#include "Algo/RandomShuffle.h"
#include "DrawManagement/Room/SpawnerComponent.h"
#include "Kismet/KismetArrayLibrary.h"
#include "PuzzleManagement/PuzzleTags.h"
#include "World/Actor/Breakable.h"

UPuzzle_Breakable::UPuzzle_Breakable()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPuzzle_Breakable::ConstructPuzzle(const FIntPoint& Coordinates)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetOwner()->GetComponents(USpawnerComponent::StaticClass(), Spawners);
	Algo::RandomShuffle(Spawners);

	const int32 PatternSelection = FMath::RandRange(0, BreakablePatterns.Num() - 1);
	FBreakablePattern Pattern = BreakablePatterns[PatternSelection];
	TMap<TSubclassOf<ABreakable>, FGameplayTag> Breakables = Pattern.GetBreakableClasses();
	
	const int32 RewardSelection = FMath::RandRange(0, Rewards.Num() - 1);
	TSubclassOf<AActor> ChosenRewardClass = Rewards[RewardSelection].GetLootItemClass();
	
	USpawnerComponent* SpawnerComponent = *Spawners.FindByPredicate([] (const USpawnerComponent* Spawner) { return Spawner->GetSpawnerTag().MatchesTagExact(Puzzle::Breakable::Pattern); });
	if (IsValid(SpawnerComponent))
	{
		GetOwner()->GetWorld()->SpawnActor<AActor>(Pattern.GetPatternClass(), SpawnerComponent->GetComponentTransform(), SpawnParams);
		Spawners.Remove(SpawnerComponent);
	}
	
	int32 SpawnerIndex = 0;
	for (const TTuple<TSubclassOf<ABreakable>, FGameplayTag>& TaggedBreakable : Breakables)
	{
		ABreakable* Breakable = GetOwner()->GetWorld()->SpawnActor<ABreakable>(TaggedBreakable.Key, Spawners[SpawnerIndex]->GetComponentTransform(), SpawnParams);
		
		if (IsValid(ChosenRewardClass) && TaggedBreakable.Value.MatchesTagExact(Puzzle::Breakable::True))
		{
			Breakable->SetLootItemClass(ChosenRewardClass);
		}
		SpawnerIndex++;
		if (SpawnerIndex >= Breakables.Num()) return;
	}
}

