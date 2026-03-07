// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "KnowledgeLog/KnowledgeTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "KnowledgeComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UKnowledgeComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UKnowledgeComponent();
	FGameplayTag GetKnowledgeCategory() const { return KnowledgeCategory; }
	FGameplayTag GetKnowledgeTag() const { return KnowledgeTag; }
	void SetKnowledgeTag(const FGameplayTag& Tag) { KnowledgeTag = Tag; }
	TArray<TInstancedStruct<FKnowledgeFragment>>& GetFragmentsMutable() { return Fragments; }
	
	template<typename T> requires std::derived_from<T, FKnowledgeFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& Tag) const;

	template<typename T> requires std::derived_from<T, FKnowledgeFragment>
	const T* GetFragmentOfType() const;

	template<typename T> requires std::derived_from<T, FKnowledgeFragment>
	T* GetFragmentOfTypeMutable();

	template<typename T> requires std::derived_from<T, FKnowledgeFragment>
	TArray<const T*> GetAllFragmentsOfType() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "DrawInventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FKnowledgeFragment>> Fragments;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Knowledge.Category"))
	FGameplayTag KnowledgeCategory = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Knowledge"))
	FGameplayTag KnowledgeTag = FGameplayTag::EmptyTag;
};

template <typename T> requires std::derived_from<T, FKnowledgeFragment>
const T* UKnowledgeComponent::GetFragmentOfTypeWithTag(const FGameplayTag& Tag) const
{
	for (const TInstancedStruct<FKnowledgeFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(Tag)) continue;
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FKnowledgeFragment>
const T* UKnowledgeComponent::GetFragmentOfType() const
{
	for (const TInstancedStruct<FKnowledgeFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FKnowledgeFragment>
T* UKnowledgeComponent::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FKnowledgeFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FKnowledgeFragment>
TArray<const T*> UKnowledgeComponent::GetAllFragmentsOfType() const
{
	TArray<const T*> Result;
	for (const TInstancedStruct<FKnowledgeFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			Result.Add(FragmentPtr);
		}
	}
	return Result;
}
