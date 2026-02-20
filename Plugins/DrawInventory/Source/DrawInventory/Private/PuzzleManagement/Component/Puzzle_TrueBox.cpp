// Retropsis 2026

#include "PuzzleManagement/Component/Puzzle_TrueBox.h"
#include "DrawManagement/Room/ItemSpawner.h"
#include "Interaction/InteractionComponent.h"
#include "Item/ItemTags.h"
#include "PuzzleManagement/PuzzleTags.h"
#include "PuzzleManagement/Piece/RewardBox.h"

UPuzzle_TrueBox::UPuzzle_TrueBox()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPuzzle_TrueBox::ConstructPuzzle()
{
	GetOwner()->GetComponents(UItemSpawner::StaticClass(), ItemSpawners);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const int32 PatternSelection = FMath::RandRange(0, TrueBoxPatterns.Num() - 1);
	FTrueBoxPattern Pattern = TrueBoxPatterns[PatternSelection];
	FGameplayTag TrueBoxTag = Pattern.GetTrueBoxTag();
	
	const int32 RewardSelection = FMath::RandRange(0, Rewards.Num() - 1);
	TSubclassOf<AActor> ChosenRewardClass = Rewards[RewardSelection].GetLootItemClass();
	
	TMap<FGameplayTag, UItemSpawner*> TaggedItemSpawners;
	for (UItemSpawner* Spawner : ItemSpawners)
	{
		TaggedItemSpawners.Add(Spawner->GetSpawnerTag(), Spawner);
	}
	
	for (const TTuple<FGameplayTag, UItemSpawner*>& Spawner : TaggedItemSpawners)
	{
		if (Spawner.Key.MatchesTagExact(Item::Puzzle::BoxKey) && IsValid(KeyItemClass) && IsValid(SpawnerClass))
		{
			ASpawner* SpawnerActor = GetWorld()->SpawnActor<ASpawner>(SpawnerClass, TaggedItemSpawners[Item::Puzzle::BoxKey]->GetComponentTransform(), SpawnParams);
			SpawnerActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
			AActor* Item = GetWorld()->SpawnActor<AActor>(KeyItemClass, SpawnerActor->GetSpawnTransform(), SpawnParams);
			Item->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
		}
				
		if (TaggedItemSpawners.Contains(Spawner.Key) && BoxClasses.Contains(Spawner.Key) && IsValid(SpawnerClass))
		{
			ASpawner* SpawnerActor = GetWorld()->SpawnActor<ASpawner>(SpawnerClass, TaggedItemSpawners[Spawner.Key]->GetComponentTransform(), SpawnParams);
			SpawnerActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
			ARewardBox* TrueBox = GetWorld()->SpawnActor<ARewardBox>(BoxClasses[Spawner.Key], SpawnerActor->GetSpawnTransform(), SpawnParams);
			TrueBox->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
		
			UInteractionComponent* InteractionComponent = TrueBox->FindComponentByClass<UInteractionComponent>();
			if (IsValid(InteractionComponent) && Pattern.GetHintMessages().Contains(Spawner.Key))
			{
				InteractionComponent->SetInteractionMessage(Pattern.GetHintMessages()[Spawner.Key]);
			}
	
			if (Spawner.Key.MatchesTagExact(TrueBoxTag) && IsValid(ChosenRewardClass))
			{
				TrueBox->SetLootItemClass(ChosenRewardClass);
			}
		}
	}
}

#if WITH_EDITOR
void UPuzzle_TrueBox::ConstructSpawners()
{
	if (!IsValid(SpawnerClass))
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
		UItemSpawner* ItemSpawner = GetOwner()->CreateDefaultSubobject<UItemSpawner>(Label);
		// UItemSpawner* ItemSpawner = NewObject<UItemSpawner>(GetOwner(), ItemSpawnerClass, Label);
		// ItemSpawner->RegisterComponent();
		// GetOwner()->AddInstanceComponent(ItemSpawner);
		ItemSpawner->SetSpawnerTag(Puzzle::Box::True);
		ItemSpawner->SetupAttachment(GetOwner()->GetRootComponent());

		ItemSpawners.Add(ItemSpawner);
	}
	
	const FName Label = FName(FString::Printf(TEXT("ItemSpawner_BoxKey")));
	UItemSpawner* ItemSpawner = GetOwner()->CreateDefaultSubobject<UItemSpawner>(Label);
	// UItemSpawner* ItemSpawner = NewObject<UItemSpawner>(GetOwner(), ItemSpawnerClass, Label);
	// ItemSpawner->RegisterComponent();
	// GetOwner()->AddInstanceComponent(ItemSpawner);
	ItemSpawner->SetSpawnerTag(Item::Puzzle::BoxKey);
	ItemSpawner->SetupAttachment(GetOwner()->GetRootComponent());

	ItemSpawners.Add(ItemSpawner);
}
#endif

