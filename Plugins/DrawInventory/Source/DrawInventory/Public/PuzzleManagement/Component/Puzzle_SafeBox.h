// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "PuzzleManagement/PuzzleComponent.h"
#include "Puzzle_SafeBox.generated.h"

class UItemSpawner;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DRAWINVENTORY_API UPuzzle_SafeBox : public UPuzzleComponent
{
	GENERATED_BODY()

public:
	UPuzzle_SafeBox();
	virtual void ConstructPuzzle() override;
	virtual void ConstructSpawners() override;

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AActor> SafeBoxClass;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AActor> CodeItemClass;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AActor> LootItemClass;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FString Code;
	
	TArray<UItemSpawner*> ItemSpawners;
};
