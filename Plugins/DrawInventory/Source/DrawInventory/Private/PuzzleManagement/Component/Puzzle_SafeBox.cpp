// Retropsis 2026

#include "PuzzleManagement/Component/Puzzle_SafeBox.h"
#include "DrawManagement/Room/ItemSpawner.h"
#include "PuzzleManagement/PuzzleTags.h"
#include "Item/ItemTags.h"
#include "Item/Component/ItemComponent.h"
#include "Item/Fragment/ItemFragment.h"
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

	const int32 PatternSelection = FMath::RandRange(0, SafeBoxPatterns.Num() - 1);
	FSafeBoxPattern Pattern = SafeBoxPatterns[PatternSelection];
	TSubclassOf<ASafeBox> ChosenSafeBoxClass = Pattern.GetSafeBoxClass();
	TSubclassOf<AActor> ChosenCodeItemClass = Pattern.GetCodeItemClass();
	
	const int32 RewardSelection = FMath::RandRange(0, Rewards.Num() - 1);
	TSubclassOf<AActor> ChosenRewardClass = Rewards[RewardSelection].GetLootItemClass();
	
	TMap<FGameplayTag, UItemSpawner*> TaggedItemSpawners;
	for (UItemSpawner* Spawner : ItemSpawners)
	{
		TaggedItemSpawners.Add(Spawner->GetSpawnerTag(), Spawner);
	}
	
	for (const TTuple<FGameplayTag, UItemSpawner*>& Spawner : TaggedItemSpawners)
	{
		if (Spawner.Key.MatchesTagExact(Item::Puzzle::Code) && IsValid(ChosenCodeItemClass))
		{
			ASpawner* SpawnerActor = GetWorld()->SpawnActor<ASpawner>(SpawnerClass, Spawner.Value->GetComponentTransform(), SpawnParams);
			SpawnerActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
			AActor* Item = GetWorld()->SpawnActor<AActor>(ChosenCodeItemClass, SpawnerActor->GetSpawnTransform(), SpawnParams);
			Item->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
			
			UItemComponent* ItemComponent = Item->FindComponentByClass<UItemComponent>();
			if (IsValid(ItemComponent))
			{
				if (FTextFragment* TextFragment = ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FTextFragment>())
				{
					FString NewText = FString::Printf(TEXT("The code is %s."), *Pattern.GetSafeBoxCode());
					TextFragment->SetText(FText::FromString(NewText));
				}
			}
		}
		if (Spawner.Key.MatchesTagExact(Puzzle::Box::Safe) && IsValid(ChosenRewardClass) && IsValid(SpawnerClass))
		{
			ASafeBox* SafeBox = GetWorld()->SpawnActor<ASafeBox>(ChosenSafeBoxClass, Spawner.Value->GetComponentTransform(), SpawnParams);
			SafeBox->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
			SafeBox->SetSafeBoxCode(Pattern.GetSafeBoxCode());
			SafeBox->SetLootItemClass(ChosenRewardClass);
		}
	}
}

void UPuzzle_SafeBox::ConstructSpawners() {}