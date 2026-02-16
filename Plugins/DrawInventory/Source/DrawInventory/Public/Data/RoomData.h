// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoomData.generated.h"

class URoomAsset;

UCLASS()
class DRAWINVENTORY_API URoomData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TMap<FIntPoint, URoomAsset*> PresetRooms;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TArray<URoomAsset*> Rooms;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	int32 Rows{9};
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	int32 Columns{5};
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	float TileSize{128};
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	int32 NumberOfDrawnRooms{3};
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	float RoomSize{8400};
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	float LockedDoorChance{50.f};
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	int32 NumberOfRedraws{1};
};
