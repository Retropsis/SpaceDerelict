// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SpawnerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API USpawnerComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	USpawnerComponent();
	FGameplayTag GetSpawnerTag() const { return SpawnerTag; }
	void SetSpawnerTag(const FGameplayTag& Tag) { SpawnerTag = Tag; }

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FGameplayTag SpawnerTag;
};
