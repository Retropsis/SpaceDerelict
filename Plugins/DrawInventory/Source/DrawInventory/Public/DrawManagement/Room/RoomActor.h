// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "RoomActor.generated.h"

class ADoor;
class ADerelictGameMode;
class UKnowledgeComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEnter, const FVector&, Location);

struct FDestinationAvailabilityResult;
class UBoxComponent;
class UDoorComponent;

UCLASS()
class DRAWINVENTORY_API ARoomActor : public AActor
{
	GENERATED_BODY()

public:
	ARoomActor();
	TMap<FName, FIntPoint>  ConstructDestinationOffsets();
	void ConstructRoom(const FDestinationAvailabilityResult& Result, float RoomSize);
	void ConstructDoors(const FDestinationAvailabilityResult& Result);
	void ConstructPuzzle(const FIntPoint& Coordinates) const;
	void ConstructDisplay(const FIntPoint& Coordinates);
	TSet<FGameplayTag>  GetLayer() const { return Layers; }
	void AddLayer(const FGameplayTag& NewLayer) { Layers.Add(NewLayer); }
	TMap<FName, FIntPoint> GetDestinationOffsets() const { return DestinationOffsets; }
	UDoorComponent* GetDoorComponentBySocket(const FName& Socket);
	TArray<FTransform> GetAvailableSpawnerTransforms() const;
	TArray<TWeakObjectPtr<ADoor>> GetDoors() const { return Doors; }
	void ConstructKnowledgeComponent(int32 Integer, UKnowledgeComponent* KnowledgeComponent, const ADerelictGameMode* DerelictGameMode);
	
	FOnPlayerEnter OnPlayerEnter;

	UFUNCTION()
	void OnRoomBoundaryBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<UDoorComponent> DoorComponentClass;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<AActor> DoorClass;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TArray<UMaterialInterface*> Numerics;

	TMap<FName, FIntPoint> DestinationOffsets;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, TObjectPtr<UDoorComponent>> DoorComponentToSockets;

	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UBoxComponent> RoomBoundary;
	
	TSet<FGameplayTag> Layers;
	TArray<TWeakObjectPtr<ADoor>> Doors;
};
