// Retropsis 2026

#include "PuzzleManagement/Piece/RewardBox.h"
#include "Game/DerelictGameMode.h"
#include "Item/Component/ItemComponent.h"
#include "Item/Fragment/ItemFragment.h"
#include "PuzzleManagement/Component/LockComponent.h"

void ARewardBox::SpawnReward()
{
	if (bHasSpawned) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (IsValid(LootItemClass))
	{
		const AActor* Item = GetWorld()->SpawnActor<AActor>(LootItemClass, GetSpawnTransform(), SpawnParams);
		
		const ADerelictGameMode* DerelictGameMode = Cast<ADerelictGameMode>(GetWorld()->GetAuthGameMode());
		check(DerelictGameMode);
		
		if (UItemComponent* ItemComponent = Item->FindComponentByClass<UItemComponent>())
		{
			const FPuzzleTagFragment* PuzzleFragment = ItemComponent->GetItemManifest().GetFragmentOfTypeWithTag<FPuzzleTagFragment>(Fragment::PuzzleTag);
			if (!PuzzleFragment) return;

			FTextFragment* TextFragment = ItemComponent->GetItemManifestMutable().GetFragmentOfTypeWithTagMutable<FTextFragment>(Fragment::FlavorText);
			if (TextFragment)
			{
				const FString Message = DerelictGameMode->GetHintMessageWithTag(PuzzleFragment->GetPuzzleTag());
				TextFragment->SetText(FText::FromString(Message));
			}
			
			FImageCollectionFragment* ImageCollectionFragment = ItemComponent->GetItemManifestMutable().GetFragmentOfTypeWithTagMutable<FImageCollectionFragment>(Fragment::ImageCollection);
			if (ImageCollectionFragment)
			{
				const TArray<UTexture2D*> ImageCollection = DerelictGameMode->GetImageCollectionWithTag(PuzzleFragment->GetPuzzleTag());
				ImageCollectionFragment->SetImageCollection(ImageCollection);
			}
		}
	}
	bHasSpawned = true;
}

void ARewardBox::BeginPlay()
{
	Super::BeginPlay();

	if (ULockComponent* LockComponent = FindComponentByClass<ULockComponent>())
	{
		LockComponent->OnUnlock.AddDynamic(this, &ThisClass::SpawnReward);
	}
 }
