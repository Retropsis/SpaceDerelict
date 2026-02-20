// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class UBoxComponent;

UCLASS()
class DRAWINVENTORY_API ASpawner : public AActor
{
	GENERATED_BODY()

public:
	ASpawner();

	FTransform GetSpawnTransform() const;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SpawnTransform;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BoxVisualizer;
};
