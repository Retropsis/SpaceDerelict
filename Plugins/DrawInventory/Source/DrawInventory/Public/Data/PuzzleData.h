// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#include "PuzzleData.generated.h"

class ASafeBox;
class ABreakable;

USTRUCT(BlueprintType)
struct FPuzzleReward
{
	GENERATED_BODY()

public:
	TSubclassOf<AActor> GetLootItemClass() const { return LootItemClass; };

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AActor> LootItemClass = nullptr;
};

USTRUCT(BlueprintType)
struct FPuzzlePattern
{
	GENERATED_BODY()

public:
	FPuzzlePattern() {}
	FPuzzlePattern(const FPuzzlePattern&) = default;
	FPuzzlePattern& operator=(const FPuzzlePattern&) = default;
	FPuzzlePattern(FPuzzlePattern&&) = default;
	FPuzzlePattern& operator=(FPuzzlePattern&&) = default;
	virtual ~FPuzzlePattern() {}

	FGameplayTag GetTag() const { return PatternTag; }
	void SetTag(const FGameplayTag& Tag) { PatternTag = Tag; }
	virtual FString GetHintMessage() const { return HintMessage; }

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Puzzle.Pattern"))
	FGameplayTag PatternTag = FGameplayTag::EmptyTag;

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FString HintMessage;
};

USTRUCT(BlueprintType)
struct FTrueBoxPattern : public FPuzzlePattern
{
	GENERATED_BODY()

public:
	FGameplayTag GetTrueBoxTag() const { return TrueBoxTag; }
	virtual FString GetHintMessage() const override;
	TMap<FGameplayTag, FString> GetHintMessages() const { return HintMessages; };
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FGameplayTag TrueBoxTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TMap<FGameplayTag, FString> HintMessages;
};

USTRUCT(BlueprintType)
struct FSafeBoxPattern : public FPuzzlePattern
{
	GENERATED_BODY()

public:
	TSubclassOf<ASafeBox> GetSafeBoxClass() const { return SafeBoxClass; }
	TSubclassOf<AActor> GetCodeItemClass() const { return CodeItemClass; }
	FString GetSafeBoxCode() const { return Code; }
	virtual FString GetHintMessage() const override;
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<ASafeBox> SafeBoxClass = nullptr;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AActor> CodeItemClass = nullptr;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FString Code{ "1234" };
};

USTRUCT(BlueprintType)
struct FDeliveryPattern : public FPuzzlePattern
{
	GENERATED_BODY()

public:
	TMap<FIntPoint, FPuzzleReward> GetRewardToCoordinates() const { return RewardToCoordinates; }
	TObjectPtr<UMaterialInterface> GetDeliveryPanel() const { return DeliveryPanel; }
	virtual FString GetHintMessage() const override;
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TMap<FIntPoint, FPuzzleReward> RewardToCoordinates;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UMaterialInterface> DeliveryPanel;
};

USTRUCT(BlueprintType)
struct FHieroglyphicCombinationPattern : public FPuzzlePattern
{
	GENERATED_BODY()

public:
	TArray<FGameplayTag> GetHieroglyphics() const { return Hieroglyphics; }
	TArray<FGameplayTag> GetHieroglyphicCombination() const { return HieroglyphicCombination; }
	TObjectPtr<UStaticMesh> GetAdditionalMesh() const { return AdditionalMesh; }
	virtual FString GetHintMessage() const override;
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<FGameplayTag> Hieroglyphics;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<FGameplayTag> HieroglyphicCombination;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UStaticMesh> AdditionalMesh = nullptr;
};

USTRUCT(BlueprintType)
struct FBreakablePattern : public FPuzzlePattern
{
	GENERATED_BODY()

public:
	TMap<TSubclassOf<ABreakable>, FGameplayTag> GetBreakableClasses() const { return BreakableClasses; }
	TSubclassOf<AActor> GetPatternClass() const { return PatternClass; }
	virtual FString GetHintMessage() const override;
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Puzzle.Breakable"))
	TMap<TSubclassOf<ABreakable>, FGameplayTag> BreakableClasses;

	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Puzzle.Breakable"))
	TSubclassOf<AActor> PatternClass;
};

UCLASS()
class DRAWINVENTORY_API UPuzzleData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	void ConstructPuzzleMeta();
	TArray<TInstancedStruct<FPuzzlePattern>> GetPuzzlePatterns() const { return  PuzzlePatterns; }

private:
	TArray<TInstancedStruct<FPuzzlePattern>> PuzzlePatterns;

	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TArray<TInstancedStruct<FTrueBoxPattern>> TrueBoxPatterns;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TArray<TInstancedStruct<FSafeBoxPattern>> SafeBoxPatterns;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TArray<TInstancedStruct<FDeliveryPattern>> DeliveryPatterns;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TArray<TInstancedStruct<FHieroglyphicCombinationPattern>> HieroglyphicCombinationPatterns;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TArray<TInstancedStruct<FBreakablePattern>> BreakablePatterns;
};
