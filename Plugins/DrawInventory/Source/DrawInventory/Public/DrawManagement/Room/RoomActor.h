// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "RoomActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnter);

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
	void ConstructRoom(const FDestinationAvailabilityResult& Result);
	void ConstructDoors(const FDestinationAvailabilityResult& Result);
	void ConstructPuzzle() const;
	void SetRoomType(const FGameplayTag& Type) { RoomType = Type; }
	TMap<FName, FIntPoint> GetDestinationOffsets() const { return DestinationOffsets; }
	UDoorComponent* GetDoorComponentBySocket(const FName& Socket);
	TArray<FTransform> GetAvailableSpawnerTransforms() const;
	
	FOnPlayerEnter OnPlayerEnter;

	UFUNCTION()
	void OnRoomBoundaryBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<UDoorComponent> DoorComponentClass;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<AActor> DoorClass;

	TMap<FName, FIntPoint> DestinationOffsets;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, TObjectPtr<UDoorComponent>> DoorComponentToSockets;

	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UBoxComponent> RoomBoundary;
	
	FGameplayTag RoomType = FGameplayTag::EmptyTag;
};
