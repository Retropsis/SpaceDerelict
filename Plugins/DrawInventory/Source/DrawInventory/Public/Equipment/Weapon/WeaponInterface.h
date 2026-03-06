// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

class AActiveEquipActor;
class AWeapon;

// This class does not need to be modified.
UINTERFACE()
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DRAWINVENTORY_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttachEquipmentMeshes(AActiveEquipActor* ActiveEquipment) = 0;
	virtual FVector GetActiveTargetLocation() = 0;
	virtual void OnSemiWeaponRefire() = 0;
	virtual void AddActiveEquipment(AActiveEquipActor* ActiveEquipment) = 0;
	virtual void HolsterActiveEquipment(AActiveEquipActor* ActiveEquipment) = 0;
	virtual void OnActiveEquipmentActivated(AActiveEquipActor* ActiveEquipment) = 0;
	virtual void OnActiveEquipmentDeactivated(AActiveEquipActor* ActiveEquipment) = 0;
};
