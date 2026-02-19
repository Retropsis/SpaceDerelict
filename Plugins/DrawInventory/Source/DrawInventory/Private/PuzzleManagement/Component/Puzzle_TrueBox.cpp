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

void UPuzzle_TrueBox::OnComponentCreated()
{
	Super::OnComponentCreated();
	ConstructSpawners();
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

#if WITH_EDITOR
void UPuzzle_TrueBox::ConstructSpawners()
{
	if (!IsValid(ItemSpawnerClass))
	{
		UE_LOG(LogTemp, Error, TEXT("ItemSpawnerClass is invalid please fill in Puzzle_TrueBox."));
		return;
	}
	
	ItemSpawners.Empty();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int32 i = 0; i < BoxClasses.Num(); ++i)
	{
		const FName Label = FName(FString::Printf(TEXT("ItemSpawner_%d"), i));
		UItemSpawner* ItemSpawner = NewObject<UItemSpawner>(GetOwner(), ItemSpawnerClass, Label);
		ItemSpawner->RegisterComponent();
		GetOwner()->AddInstanceComponent(ItemSpawner);
		ItemSpawner->SetSpawnerTag(Puzzle::Box::True);
		ItemSpawner->SetupAttachment(GetOwner()->GetRootComponent());

		ItemSpawners.Add(ItemSpawner);
	}
	
	const FName Label = FName(FString::Printf(TEXT("ItemSpawner_BoxKey")));
	UItemSpawner* ItemSpawner = NewObject<UItemSpawner>(GetOwner(), ItemSpawnerClass, Label);
	ItemSpawner->RegisterComponent();
	GetOwner()->AddInstanceComponent(ItemSpawner);
	ItemSpawner->SetSpawnerTag(Item::Currency::BoxKey);
	ItemSpawner->SetupAttachment(GetOwner()->GetRootComponent());

	ItemSpawners.Add(ItemSpawner);
}
#endif

