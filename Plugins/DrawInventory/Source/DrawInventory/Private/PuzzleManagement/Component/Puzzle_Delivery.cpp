// Retropsis 2026

#include "PuzzleManagement/Component/Puzzle_Delivery.h"
#include "Data/PuzzleData.h"
#include "Game/DerelictGameMode.h"
#include "PuzzleManagement/PuzzleTags.h"
#include "PuzzleManagement/Piece/DeliveryBox.h"

UPuzzle_Delivery::UPuzzle_Delivery()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPuzzle_Delivery::ConstructPuzzle(const FIntPoint& Coordinates)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const ADerelictGameMode* DerelictGameMode = Cast<ADerelictGameMode>(GetWorld()->GetAuthGameMode());
	check(DerelictGameMode);

	const FDeliveryPattern* Pattern = DerelictGameMode->GetPuzzleDataOfTypeWithTag<FDeliveryPattern>(Puzzle::Pattern::Delivery);
	
	TSubclassOf<AActor> ItemToSpawn = nullptr;
	for (const TTuple<FIntPoint, FPuzzleReward>& Pair : Pattern->GetRewardToCoordinates())
	{
		if (Pair.Key == Coordinates)
		{
			ItemToSpawn = Pair.Value.GetLootItemClass();
			break;
		}
	}
	
	for (const TObjectPtr<AActor>& Actor : GetOwner()->GetLevel()->Actors)
	{
		if (ADeliveryBox* DeliveryBox = Cast<ADeliveryBox>(Actor))
		{
			DeliveryBox->SetLootItemClass(ItemToSpawn);
			DeliveryBox->SetupDeliveryPanel(DerelictGameMode->ConstructDeliveryPanel(*Pattern));
			return;
		}
	}
}

