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

void UPuzzle_TrueBox::InitializeComponent()
{
	Super::InitializeComponent();
	// ConstructSpawners();
}

void UPuzzle_TrueBox::ConstructPuzzle()
{
	GetOwner()->GetComponents(ItemSpawnerClass, ItemSpawners);

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
		if (Spawner.Key.MatchesTagExact(Item::Puzzle::BoxKey) && IsValid(KeyItemClass))
		{
			GetWorld()->SpawnActor<AActor>(KeyItemClass, TaggedItemSpawners[Item::Puzzle::BoxKey]->GetComponentTransform(), SpawnParams);
		}
				
		if (TaggedItemSpawners.Contains(Spawner.Key) && BoxClasses.Contains(Spawner.Key))
		{
			ARewardBox* TrueBox = GetWorld()->SpawnActor<ARewardBox>(BoxClasses[Spawner.Key], TaggedItemSpawners[Spawner.Key]->GetComponentTransform(), SpawnParams);
		
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

