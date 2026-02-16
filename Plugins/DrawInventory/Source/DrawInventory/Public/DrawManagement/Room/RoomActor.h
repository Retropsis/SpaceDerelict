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
	void SetRoomType(const FGameplayTag& Type) { RoomType = Type; }
	TMap<FName, FIntPoint> GetDestinationOffsets() const { return DestinationOffsets; }

private:
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<UDoorComponent> DoorComponentClass;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<AActor> DoorClass;

	TMap<FName, FIntPoint> DestinationOffsets;
	
	FGameplayTag RoomType = FGameplayTag::EmptyTag;
};
