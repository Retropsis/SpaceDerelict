// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "PuzzleManagement/PuzzleComponent.h"
#include "Puzzle_TrueBox.generated.h"

class UItemSpawner;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DRAWINVENTORY_API UPuzzle_TrueBox : public UPuzzleComponent
{
	GENERATED_BODY()

public:
	UPuzzle_TrueBox();
	virtual void ConstructPuzzle() override;
	virtual void ConstructSpawners() override;

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<UItemSpawner> ItemSpawnerClass;

	TArray<UItemSpawner*> ItemSpawners;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AActor> KeyItemClass;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AActor> LootItemClass;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<TSubclassOf<AActor>> BoxClasses;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TArray<FString> HintMessages;
};
