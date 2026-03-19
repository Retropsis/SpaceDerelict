// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UPuzzleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void ConstructPuzzle(const FIntPoint& Coordinates) {}
	virtual void ConstructSpawners() {}
};
