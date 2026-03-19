// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "StructUtils/InstancedStruct.h"
#include "DerelictGameMode.generated.h"

struct FPuzzlePattern;
class UPuzzleData;
class UKnowledgeData;

UCLASS()
class DRAWINVENTORY_API ADerelictGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UKnowledgeData* GetKnowledgeData() const { return KnowledgeData; }

	template<typename T> requires std::derived_from<T, FPuzzlePattern>
	const T* GetPuzzleDataOfType() const;
	
	template<typename T> requires std::derived_from<T, FPuzzlePattern>
	const T* GetPuzzleDataOfTypeWithTag(const FGameplayTag& Tag) const;

	FString GetHintMessageWithTag(const FGameplayTag& Tag) const;

protected:
	virtual void BeginPlay() override;
	
private:
	void ConstructPuzzleMeta();
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UKnowledgeData> KnowledgeData;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UPuzzleData> PuzzleInfo;

	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TArray<TInstancedStruct<FPuzzlePattern>> PuzzleData;
};

template <typename T> requires std::derived_from<T, FPuzzlePattern>
const T* ADerelictGameMode::GetPuzzleDataOfType() const
{
	for (const auto& Data : PuzzleData)
	{
		if (const T* DataPtr = Data.GetPtr<T>())
		{
			return DataPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FPuzzlePattern>
const T* ADerelictGameMode::GetPuzzleDataOfTypeWithTag(const FGameplayTag& Tag) const
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
