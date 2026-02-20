// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SceneComponent.h"
#include "ItemSpawner.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UItemSpawner : public USceneComponent
{
	GENERATED_BODY()

public:
	FGameplayTag GetSpawnerTag() const { return SpawnerTag; }
	void SetSpawnerTag(const FGameplayTag& Tag) { SpawnerTag = Tag; }

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FGameplayTag SpawnerTag;
};
