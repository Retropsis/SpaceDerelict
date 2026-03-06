// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Equipment/ActiveEquipActor.h"
#include "EquipmentManagement/EquipActor/EquipActor.h"
#include "Weapon.generated.h"

class UInventoryComponent;
class AProjectile;
class IWeaponInterface;

UCLASS()
class DRAWINVENTORY_API AWeapon : public AActiveEquipActor
{
	GENERATED_BODY()

public:
	virtual void InitializeEquipment() override;
	virtual void StartActive() override;
	virtual void StopActive() override;
	
	UFUNCTION()
	void OnBulletCountChange(const FGameplayTag& ItemType, int32 Amount);

private:	
	virtual void Fire();
	virtual void FireProjectile(const FVector& TargetLocation);
	void FireCooldownExpired();
	FTransform CalculateProjectileSpawnTransform(const FVector& TargetLocation) const;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UAnimMontage> FiringMontage;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	bool bFullAuto = false;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float FireRate = 0.5f;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, Category="DrawInventory", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float MuzzleOffset = 10.0f;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta = (ClampMin = 0, ClampMax = 90, Units = "Degrees"))
	float AimVariance = 0.0f;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta = (ClampMin = 0, ClampMax = 100))
	int32 MagazineSize = 10;

	int32 CurrentBullets = 0;
	
	bool bIsFiring = false;
	float TimeOfLastShot = 0.0f;
	FTimerHandle RefireTimer;

	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
};
