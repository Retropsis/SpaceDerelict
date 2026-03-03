// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

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
	virtual void AttachWeaponMeshes(AWeapon* Weapon) = 0;
	virtual FVector GetWeaponTargetLocation() = 0;
	virtual void OnSemiWeaponRefire() = 0;
	virtual void AddWeapon(AWeapon* Weapon) = 0;
	virtual void HolsterWeapon(AWeapon* Weapon) = 0;
	virtual void OnWeaponActivated(AWeapon* Weapon) = 0;
	virtual void OnWeaponDeactivated(AWeapon* Weapon) = 0;
};
