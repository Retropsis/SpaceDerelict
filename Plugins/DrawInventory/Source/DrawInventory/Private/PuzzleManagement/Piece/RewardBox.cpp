// Retropsis 2026

#include "PuzzleManagement/Piece/RewardBox.h"
#include "PuzzleManagement/Component/LockComponent.h"

void ARewardBox::SpawnReward()
{
	if (bHasSpawned) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (IsValid(LootItemClass))
	{
		GetWorld()->SpawnActor<AActor>(LootItemClass, GetSpawnTransform(), SpawnParams);
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
