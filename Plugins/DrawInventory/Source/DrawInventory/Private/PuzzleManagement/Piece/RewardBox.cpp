// Retropsis 2026

#include "PuzzleManagement/Piece/RewardBox.h"
#include "DrawManagement/Room/ItemSpawner.h"
#include "PuzzleManagement/Component/LockComponent.h"

void ARewardBox::SpawnReward()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (const UItemSpawner* ItemSpawner = FindComponentByClass<UItemSpawner>(); IsValid(ItemSpawner) && IsValid(LootItemClass))
	{
		GetWorld()->SpawnActor<AActor>(LootItemClass, ItemSpawner->GetComponentTransform(), SpawnParams);
	}
}

void ARewardBox::BeginPlay()
{
	Super::BeginPlay();

	if (ULockComponent* LockComponent = FindComponentByClass<ULockComponent>())
	{
		LockComponent->OnUnlock.AddDynamic(this, &ThisClass::SpawnReward);
	}
 }
