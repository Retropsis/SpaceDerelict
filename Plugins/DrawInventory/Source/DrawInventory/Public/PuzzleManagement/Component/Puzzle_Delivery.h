// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "PuzzleManagement/PuzzleComponent.h"
#include "Puzzle_Delivery.generated.h"

struct FDeliveryPattern;
class USpawnerComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class DRAWINVENTORY_API UPuzzle_Delivery : public UPuzzleComponent
{
	GENERATED_BODY()

public:
	UPuzzle_Delivery();
	virtual void ConstructPuzzle(const FIntPoint& Coordinates) override;
};
