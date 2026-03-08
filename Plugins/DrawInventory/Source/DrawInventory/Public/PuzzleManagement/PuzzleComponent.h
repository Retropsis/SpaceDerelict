// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PuzzleComponent.generated.h"

class ABreakable;
class ASafeBox;

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
	const FGameplayTag& GetHintMessage() const { return HintMessage; }

private:
	FGameplayTag HintMessage;
};

USTRUCT(BlueprintType)
struct FTrueBoxPattern
{
	GENERATED_BODY()

public:
	FGameplayTag GetTrueBoxTag() const { return TrueBoxTag; }
	TMap<FGameplayTag, FString> GetHintMessages() const { return HintMessages; }
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FGameplayTag TrueBoxTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TMap<FGameplayTag, FString> HintMessages;
};

USTRUCT(BlueprintType)
struct FSafeBoxPattern
{
	GENERATED_BODY()

public:
	TSubclassOf<ASafeBox> GetSafeBoxClass() const { return SafeBoxClass; }
	TSubclassOf<AActor> GetCodeItemClass() const { return CodeItemClass; }
	FString GetSafeBoxCode() const { return Code; }
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<ASafeBox> SafeBoxClass = nullptr;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AActor> CodeItemClass = nullptr;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FString Code{ "1234" };
};

USTRUCT(BlueprintType)
struct FDeliveryPattern
{
	GENERATED_BODY()

public:
	TMap<FIntPoint, FPuzzleReward> GetRewardToCoordinates() const { return RewardToCoordinates; }
	TObjectPtr<UMaterialInterface> GetDeliveryPanel() const { return DeliveryPanel; }
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TMap<FIntPoint, FPuzzleReward> RewardToCoordinates;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UMaterialInterface> DeliveryPanel;
};

USTRUCT(BlueprintType)
struct FHieroglyphicCombinationPattern
{
	GENERATED_BODY()

public:
	TArray<FGameplayTag> GetHieroglyphics() const { return Hieroglyphics; }
	TArray<FGameplayTag> GetHieroglyphicCombination() const { return HieroglyphicCombination; }
	TObjectPtr<UStaticMesh> GetAdditionalMesh() const { return AdditionalMesh; }
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<FGameplayTag> Hieroglyphics;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<FGameplayTag> HieroglyphicCombination;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UStaticMesh> AdditionalMesh = nullptr;
};

USTRUCT(BlueprintType)
struct FBreakablePattern
{
	GENERATED_BODY()

public:
	TMap<TSubclassOf<ABreakable>, FGameplayTag> GetBreakableClasses() const { return BreakableClasses; }
	TSubclassOf<AActor> GetPatternClass() const { return PatternClass; }
	
private:
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Puzzle.Breakable"))
	TMap<TSubclassOf<ABreakable>, FGameplayTag> BreakableClasses;

	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Puzzle.Breakable"))
	TSubclassOf<AActor> PatternClass;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UPuzzleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void ConstructPuzzle(const FIntPoint& Coordinates) {}
	virtual void ConstructSpawners() {}
};
