// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "PuzzleManagement/PuzzleComponent.h"
#include "Puzzle_TrueBox.generated.h"

class ARewardBox;
class UItemSpawner;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DRAWINVENTORY_API UPuzzle_TrueBox : public UPuzzleComponent
{
	GENERATED_BODY()

public:
	UPuzzle_TrueBox();
	
#if WITH_EDITOR
	virtual void InitializeComponent() override;
#endif
	
	virtual void ConstructPuzzle() override;
	virtual void ConstructSpawners() override;

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<UItemSpawner> ItemSpawnerClass;

	TArray<UItemSpawner*> ItemSpawners;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AActor> KeyItemClass;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TMap<FGameplayTag, TSubclassOf<ARewardBox>> BoxClasses;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<FTrueBoxPattern> TrueBoxPatterns;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<FPuzzleReward> Rewards;
};
