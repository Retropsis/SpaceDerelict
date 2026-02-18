// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "RoomActor.generated.h"

struct FDestinationAvailabilityResult;
class UDoorComponent;

UCLASS()
class DRAWINVENTORY_API ARoomActor : public AActor
{
	GENERATED_BODY()

public:
	ARoomActor();
	TMap<FName, FIntPoint>  ConstructDestinationOffsets();
	void ConstructDoors(const FDestinationAvailabilityResult& Result);
	void ConstructPuzzle() const;
	void SetRoomType(const FGameplayTag& Type) { RoomType = Type; }
	TMap<FName, FIntPoint> GetDestinationOffsets() const { return DestinationOffsets; }
	UDoorComponent* GetDoorComponentBySocket(const FName& Socket);
	TArray<FTransform> GetAvailableSpawnerTransforms() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<UDoorComponent> DoorComponentClass;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<AActor> DoorClass;

	TMap<FName, FIntPoint> DestinationOffsets;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, TObjectPtr<UDoorComponent>> DoorComponentToSockets;
	
	FGameplayTag RoomType = FGameplayTag::EmptyTag;
};
