// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item/Manifest/ItemManifest.h"
#include "RoomAsset.generated.h"


UCLASS()
class DRAWINVENTORY_API URoomAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void InitializeRoomManifest(FItemManifest CopyOfRoomManifest);
	FItemManifest GetRoomManifest() const { return RoomManifest; }
	FItemManifest& GetRoomManifestMutable() { return RoomManifest; }

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FItemManifest RoomManifest;
};
