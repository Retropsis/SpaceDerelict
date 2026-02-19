// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PuzzleComponent.generated.h"

class ASafeBox;

USTRUCT(BlueprintType)
struct FTrueBoxPattern
{
	GENERATED_BODY()

public:
	FGameplayTag GetTrueBoxTag() const { return TrueBoxTag; }
	TMap<FGameplayTag, FString> GetHintMessages() const { return HintMessages; };
	
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
struct FPuzzleReward
{
	GENERATED_BODY()

public:
	TSubclassOf<AActor> GetLootItemClass() const { return LootItemClass; };

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AActor> LootItemClass = nullptr;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UPuzzleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void ConstructPuzzle() {}
	virtual void ConstructSpawners() {}
};
