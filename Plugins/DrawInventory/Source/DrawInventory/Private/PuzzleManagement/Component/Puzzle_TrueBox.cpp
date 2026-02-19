// Retropsis 2026

#include "PuzzleManagement/Component/Puzzle_TrueBox.h"

#include "DrawManagement/Room/ItemSpawner.h"
#include "Interaction/InteractionComponent.h"
#include "Item/ItemTags.h"
#include "PuzzleManagement/PuzzleTags.h"

UPuzzle_TrueBox::UPuzzle_TrueBox()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPuzzle_TrueBox::ConstructPuzzle()
{
	GetOwner()->GetComponents(ItemSpawnerClass, ItemSpawners);

	int32 BoxIndex = 0;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (const TObjectPtr<UItemSpawner>& Spawner : ItemSpawners)
	{
		if (Spawner->GetSpawnerTag().MatchesTagExact(Item::Currency::BoxKey))
		{
			GetWorld()->SpawnActor<AActor>(KeyItemClass, Spawner->GetComponentTransform(), SpawnParams);
		}
		if (Spawner->GetSpawnerTag().MatchesTagExact(Puzzle::Box::True))
		{
			if (!BoxClasses.IsValidIndex(BoxIndex)) continue;
			
			AActor* TrueBox = GetWorld()->SpawnActor<AActor>(BoxClasses[BoxIndex], Spawner->GetComponentTransform(), SpawnParams);

			if (BoxIndex == 0)	AActor* Loot = GetWorld()->SpawnActor<AActor>(LootItemClass, Spawner->GetComponentTransform(), SpawnParams);
			
			UInteractionComponent* InteractionComponent = TrueBox->FindComponentByClass<UInteractionComponent>();
			if (IsValid(InteractionComponent))
			{
				InteractionComponent->SetInteractionMessage(HintMessages[BoxIndex]);
			}
			BoxIndex++;
		}
	}
}

void UPuzzle_TrueBox::ConstructSpawners()
{
}

