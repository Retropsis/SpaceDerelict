#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "ItemManifest.generated.h"

class ARoomActor;
class ALevelInstance;
class UCompositeBase;
struct FItemFragment;

/*
 * Item Manifest contains all necessary data to create a new item
 */
USTRUCT(BlueprintType)
struct DRAWINVENTORY_API FItemManifest
{
	GENERATED_BODY()

public:
	TArray<TInstancedStruct<FItemFragment>>& GetFragmentsMutable() { return Fragments; }
	UInventoryItem* Manifest(UObject* NewOuter);
	EGridCategory GetItemCategory() const { return ItemCategory; }
	FGameplayTag GetItemType() const { return ItemType; }
	void SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation);
	ULevelStreaming* LoadRoom(UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation, bool& bOutSuccess) const;
	void AssimilateInventoryFragments(UCompositeBase* Composite) const;

	template<typename T> requires std::derived_from<T, FItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& Tag) const;

	template<typename T> requires std::derived_from<T, FItemFragment>
	const T* GetFragmentOfType() const;

	template<typename T> requires std::derived_from<T, FItemFragment>
	T* GetFragmentOfTypeMutable();

	template<typename T> requires std::derived_from<T, FItemFragment>
	TArray<const T*> GetAllFragmentsOfType() const;

private:
	void ClearFragments();
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FItemFragment>> Fragments;
		
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	EGridCategory ItemCategory{EGridCategory::None};

	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Item"))
	FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AActor> PickupActorClass;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSoftObjectPtr<UWorld> RoomLevel;
};

template<typename T> requires std::derived_from<T, FItemFragment>
const T* FItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& Tag) const
{
	for (const TInstancedStruct<FItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(Tag)) continue;
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FItemFragment>
const T* FItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FItemFragment>
T* FItemManifest::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FItemFragment>
TArray<const T*> FItemManifest::GetAllFragmentsOfType() const
{
	TArray<const T*> Result;
	for (const TInstancedStruct<FItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			Result.Add(FragmentPtr);
		}
	}
	return Result;
}
