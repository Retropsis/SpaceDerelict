// Retropsis 2026

#include "PuzzleManagement/Component/Puzzle_SafeBox.h"
#include "Data/PuzzleData.h"
#include "DrawManagement/Room/SpawnerComponent.h"
#include "Game/DerelictGameMode.h"
#include "PuzzleManagement/PuzzleTags.h"
#include "Item/ItemTags.h"
#include "PuzzleManagement/Piece/SafeBox.h"

UPuzzle_SafeBox::UPuzzle_SafeBox()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPuzzle_SafeBox::ConstructPuzzle(const FIntPoint& Coordinates)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetOwner()->GetComponents(USpawnerComponent::StaticClass(), ItemSpawners);

	const ADerelictGameMode* DerelictGameMode = Cast<ADerelictGameMode>(GetWorld()->GetAuthGameMode());
	check(DerelictGameMode);

	const FSafeBoxPattern* Pattern = DerelictGameMode->GetPuzzleDataOfTypeWithTag<FSafeBoxPattern>(Puzzle::Pattern::SafeBox);
	const TSubclassOf<ASafeBox> ChosenSafeBoxClass = Pattern->GetSafeBoxClass();
	
	const int32 RewardSelection = FMath::RandRange(0, Rewards.Num() - 1);
	const TSubclassOf<AActor> ChosenRewardClass = Rewards[RewardSelection].GetLootItemClass();
	
	TMap<FGameplayTag, USpawnerComponent*> TaggedItemSpawners;
	for (USpawnerComponent* Spawner : ItemSpawners)
	{
		TaggedItemSpawners.Add(Spawner->GetSpawnerTag(), Spawner);
	}
	
	for (const TTuple<FGameplayTag, USpawnerComponent*>& Spawner : TaggedItemSpawners)
	{
		if (Spawner.Key.MatchesTagExact(Puzzle::Box::Safe) && IsValid(ChosenRewardClass) && IsValid(SpawnerClass))
		{
			ASafeBox* SafeBox = GetWorld()->SpawnActor<ASafeBox>(ChosenSafeBoxClass, Spawner.Value->GetComponentTransform(), SpawnParams);
			SafeBox->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
			SafeBox->SetSafeBoxCode(Pattern->GetSafeBoxCode());
			SafeBox->SetLootItemClass(ChosenRewardClass);
		}
	}
}

void UPuzzle_SafeBox::ConstructSpawners() {}