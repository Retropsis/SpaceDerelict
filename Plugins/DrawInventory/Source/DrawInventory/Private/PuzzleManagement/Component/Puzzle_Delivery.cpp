// Retropsis 2026

#include "PuzzleManagement/Component/Puzzle_Delivery.h"
#include "DrawManagement/Room/SpawnerComponent.h"
#include "PuzzleManagement/Piece/RewardBox.h"

UPuzzle_Delivery::UPuzzle_Delivery()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPuzzle_Delivery::ConstructPuzzle(const FIntPoint& Coordinates)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const int32 PatternSelection = FMath::RandRange(0, DeliveryPatterns.Num() - 1);
	FDeliveryPattern Pattern = DeliveryPatterns[PatternSelection];

	if (!Pattern.GetRewardToCoordinates().Contains(Coordinates)) return;
	
	FPuzzleReward* Reward = Pattern.GetRewardToCoordinates().Find(Coordinates);
	TSubclassOf<AActor> ItemToSpawn = Reward->GetLootItemClass();

	for (const TObjectPtr<AActor>& Actor : GetOwner()->GetLevel()->Actors)
	{
		if (ARewardBox* RewardBox = Cast<ARewardBox>(Actor))
		{
			RewardBox->SetLootItemClass(ItemToSpawn);
			RewardBox->SetResourceObject(Pattern.GetDeliveryPanel());
			return;
		}
	}
}

