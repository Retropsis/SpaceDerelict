// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewardBox.generated.h"


UCLASS()
class DRAWINVENTORY_API ARewardBox : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SpawnReward();
	
	void SetLootItemClass(const TSubclassOf<AActor>& Class) { LootItemClass = Class; }

protected:
	virtual void BeginPlay() override;

private:
	TSubclassOf<AActor> LootItemClass;
};
