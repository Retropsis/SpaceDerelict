// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "PuzzleManagement/PuzzleComponent.h"
#include "Puzzle_SafeBox.generated.h"

struct FSafeBoxPattern;
struct FPuzzleReward;
class ASpawner;
class USpawnerComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DRAWINVENTORY_API UPuzzle_SafeBox : public UPuzzleComponent
{
	GENERATED_BODY()

public:
	UPuzzle_SafeBox();
	virtual void ConstructPuzzle(const FIntPoint& Coordinates) override;
	virtual void ConstructSpawners() override;

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<ASpawner> SpawnerClass;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<FSafeBoxPattern> SafeBoxPatterns;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<FPuzzleReward> Rewards;
	
	TArray<USpawnerComponent*> ItemSpawners;
};
