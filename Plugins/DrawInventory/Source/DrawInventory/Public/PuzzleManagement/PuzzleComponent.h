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
	UPuzzleComponent();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnComponentCreated() override;
	virtual void ConstructPuzzle();
	virtual void ConstructSpawners();
};
