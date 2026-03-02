// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "EquipmentManagement/EquipActor/EquipActor.h"
#include "Weapon.generated.h"

class UInventoryComponent;
class AProjectile;
class IWeaponInterface;

UCLASS()
class DRAWINVENTORY_API AWeapon : public AEquipActor
{
	GENERATED_BODY()

public:
	AWeapon();
	void StartFiring();
	void StopFiring();
	void ActivateWeapon();
	void DeactivateWeapon();
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	const TSubclassOf<UAnimInstance>& GetFirstPersonAnimInstanceClass() const { return FirstPersonAnimInstanceClass; }
	virtual void OnEquip(APawn* Pawn) override;
	virtual void InitializeEquipment() override;

	UFUNCTION()
	void OnBulletCountChange(const FGameplayTag& ItemType, int32 Amount);

private:
	virtual void Fire();
	virtual void FireProjectile(const FVector& TargetLocation);
	void FireCooldownExpired();
	FTransform CalculateProjectileSpawnTransform(const FVector& TargetLocation) const;
	
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<UAnimInstance> FirstPersonAnimInstanceClass;
	
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
	IWeaponInterface* WeaponOwner = nullptr;

	UPROPERTY()
	TObjectPtr<APawn> PawnOwner;

	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
};
