// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "PuzzleManagement/PuzzleComponent.h"
#include "Puzzle_Breakable.generated.h"


struct FPuzzleReward;
struct FBreakablePattern;
class USpawnerComponent;
class ABreakable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DRAWINVENTORY_API UPuzzle_Breakable : public UPuzzleComponent
{
	GENERATED_BODY()

public:
	UPuzzle_Breakable();
	virtual void ConstructPuzzle(const FIntPoint& Coordinates) override;

private:		
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<FPuzzleReward> Rewards;
	
	TArray<USpawnerComponent*> Spawners;
};
