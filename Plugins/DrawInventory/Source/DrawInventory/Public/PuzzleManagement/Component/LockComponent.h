// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Interaction/InteractionComponent.h"
#include "LockComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnlock);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API ULockComponent : public UInteractionComponent
{
	GENERATED_BODY()

public:
	ULockComponent();

	void Unlock();
	FGameplayTag GetLockType() const { return LockType; }

	UPROPERTY(BlueprintAssignable, Category="DrawInventory")
	FUnlock OnUnlock;

private:
	UPROPERTY(EditAnywhere, meta=(Categories="Item.Currency"))
	FGameplayTag LockType = FGameplayTag::EmptyTag;
};
