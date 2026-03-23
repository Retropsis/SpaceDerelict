// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PuzzleManagement/PuzzleComponent.h"
#include "Puzzle_HieroglyphicCombination.generated.h"

struct FHieroglyphicCombinationPattern;
class APressurePlate;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombinationComplete);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class DRAWINVENTORY_API UPuzzle_HieroglyphicCombination : public UPuzzleComponent
{
	GENERATED_BODY()

public:
	UPuzzle_HieroglyphicCombination();

	UFUNCTION()
	void OnSymbolReceived(const FGameplayTag& Symbol);

	UPROPERTY(BlueprintAssignable)
	FOnCombinationComplete OnCombinationComplete;

protected:
	virtual void ConstructPuzzle(const FIntPoint& Coordinates) override;

private:	
	TArray<APressurePlate> PressurePlates;
	TArray<FGameplayTag> DesiredCombination;
	TArray<FGameplayTag> InputCombination;
};
