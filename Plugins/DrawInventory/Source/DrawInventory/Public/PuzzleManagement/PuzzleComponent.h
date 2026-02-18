// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleComponent.generated.h"


class UItemSpawner;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UPuzzleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPuzzleComponent();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnComponentCreated() override;
	virtual void ConstructPuzzle();
	void ConstructSpawners();

protected:

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<UItemSpawner> ItemSpawnerClass;

	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
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
