// Retropsis 2026

#include "PuzzleManagement/Component/Puzzle_SafeBox.h"
#include "DrawManagement/Room/ItemSpawner.h"
#include "Item/ItemTags.h"
#include "PuzzleManagement/PuzzleTags.h"
#include "PuzzleManagement/Piece/SafeBox.h"

UPuzzle_SafeBox::UPuzzle_SafeBox()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPuzzle_SafeBox::ConstructPuzzle()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetOwner()->GetComponents(UItemSpawner::StaticClass(), ItemSpawners);

	for (const UItemSpawner* Spawner : ItemSpawners)
	{
		if (Spawner->GetSpawnerTag().MatchesTagExact(Item::Puzzle::Code) && IsValid(CodeItemClass))
		{
			GetWorld()->SpawnActor<AActor>(CodeItemClass, Spawner->GetComponentTransform(), SpawnParams);
		}
		if (Spawner->GetSpawnerTag().MatchesTagExact(Puzzle::Box::Safe) && IsValid(SafeBoxClass))
		{
			ASafeBox* SafeBox = GetWorld()->SpawnActor<ASafeBox>(SafeBoxClass, Spawner->GetComponentTransform(), SpawnParams);
			if (UItemSpawner* ItemSpawner = SafeBox->FindComponentByClass<UItemSpawner>(); IsValid(ItemSpawner) && IsValid(LootItemClass))
			{
				GetWorld()->SpawnActor<AActor>(LootItemClass, ItemSpawner->GetComponentTransform(), SpawnParams);
			}
		}
	}
}

void UPuzzle_SafeBox::ConstructSpawners()
{
	
}

