// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "StructUtils/InstancedStruct.h"
#include "DerelictGameMode.generated.h"

class UKnowledgeData;

USTRUCT()
struct FPuzzleData
{
	GENERATED_BODY()

public:
	FPuzzleData() {}
	FPuzzleData(const FPuzzleData&) = default;
	FPuzzleData& operator=(const FPuzzleData&) = default;
	FPuzzleData(FPuzzleData&&) = default;
	FPuzzleData& operator=(FPuzzleData&&) = default;
	virtual ~FPuzzleData() {}
	
	FGameplayTag GetTag() const { return Tag; }
	void SetTag(const FGameplayTag& NewTag) { Tag = NewTag; }
	
private:
	FGameplayTag Tag = FGameplayTag::EmptyTag;
};

USTRUCT()
struct FDataString : public FPuzzleData
{
	GENERATED_BODY()

public:
	FString GetDataString() const { return DataString; }
	void SetDataString(const FString& NewDataString) { DataString = NewDataString; }
	
private:
	FString DataString = FString();
};

USTRUCT()
struct FTagCombinationData : public FPuzzleData
{
	GENERATED_BODY()

public:
	TArray<FGameplayTag> GetTagCombination() const { return TagCombination; }
	void SetTagCombination(const TArray<FGameplayTag>& NewCombination) { TagCombination = NewCombination; }
	
private:
	TArray<FGameplayTag> TagCombination;
};


UCLASS()
class DRAWINVENTORY_API ADerelictGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UKnowledgeData* GetKnowledgeData() const { return KnowledgeData; }

	template<typename T> requires std::derived_from<T, FPuzzleData>
	const T* GetPuzzleDataOfType(const FGameplayTag& Tag) const;

	void AddPuzzleData(const TInstancedStruct<FPuzzleData>& Data);
	bool IsPuzzleDataValid(const FGameplayTag& PuzzleTag) const;
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UKnowledgeData> KnowledgeData;

	TArray<TInstancedStruct<FPuzzleData>> PuzzleData;
};

template <typename T> requires std::derived_from<T, FPuzzleData>
const T* ADerelictGameMode::GetPuzzleDataOfType(const FGameplayTag& Tag) const
{
	for (const auto& Data : PuzzleData)
	{
		if (const T* DataPtr = Data.GetPtr<T>())
		{
			if (!DataPtr->GetTag().MatchesTagExact(Tag)) continue;
			return DataPtr;
		}
	}
	return nullptr;
}
