// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "PuzzleManagement/PuzzleComponent.h"
#include "Puzzle_HieroglyphicCombination.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class DRAWINVENTORY_API UPuzzle_HieroglyphicCombination : public UPuzzleComponent
{
	GENERATED_BODY()

public:
	UPuzzle_HieroglyphicCombination();

protected:
	virtual void ConstructPuzzle(const FIntPoint& Coordinates) override;
};
