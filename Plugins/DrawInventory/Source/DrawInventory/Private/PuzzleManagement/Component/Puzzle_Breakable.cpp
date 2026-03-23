// Retropsis 2026

#include "PuzzleManagement/Component/Puzzle_Breakable.h"
#include "Algo/RandomShuffle.h"
#include "DrawManagement/Room/SpawnerComponent.h"
#include "Game/DerelictGameMode.h"
#include "Kismet/KismetArrayLibrary.h"
#include "PuzzleManagement/PuzzleTags.h"
#include "Data/PuzzleData.h"
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
	
	const ADerelictGameMode* DerelictGameMode = Cast<ADerelictGameMode>(GetWorld()->GetAuthGameMode());
	check(DerelictGameMode);

	const FBreakablePattern* Pattern = DerelictGameMode->GetPuzzleDataOfTypeWithTag<FBreakablePattern>(Puzzle::Pattern::Breakable);
	TMap<TSubclassOf<ABreakable>, FGameplayTag> Breakables = Pattern->GetBreakableClasses();
	
	const int32 RewardSelection = FMath::RandRange(0, Rewards.Num() - 1);
	const TSubclassOf<AActor> ChosenRewardClass = Rewards[RewardSelection].GetLootItemClass();
	
	USpawnerComponent* SpawnerComponent = nullptr;
	for (USpawnerComponent* Spawner : Spawners)
	{
		if (IsValid(Spawner) && Spawner->GetSpawnerTag().MatchesTagExact(Puzzle::Pattern::Breakable))
		{
			SpawnerComponent = Spawner;
			break;
		}
	}
	
	if (IsValid(SpawnerComponent))
	{
		GetOwner()->GetWorld()->SpawnActor<AActor>(Pattern->GetPatternClass(), SpawnerComponent->GetComponentTransform(), SpawnParams);
		Spawners.Remove(SpawnerComponent);
	}
	
	Algo::RandomShuffle(Spawners);
	
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

