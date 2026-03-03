// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Weapon/WeaponInterface.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class APlayerCharacterController;
class UCameraComponent;

UCLASS()
class DRAWINVENTORY_API APlayerCharacter : public ACharacter, public IWeaponInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	
	//~ WeaponInterface
	virtual void AttachWeaponMeshes(AWeapon* Weapon) override;
	virtual FVector GetWeaponTargetLocation() override;
	virtual void OnSemiWeaponRefire() override;
	virtual void AddWeapon(AWeapon* Weapon) override;
	virtual void HolsterWeapon(AWeapon* Weapon) override;
	virtual void OnWeaponActivated(AWeapon* Weapon) override;
	virtual void OnWeaponDeactivated(AWeapon* Weapon) override;
	//~ WeaponInterface

	UFUNCTION(BlueprintImplementableEvent)
	void Test();
	
	void StartFiring() const;
	void StopFiring() const;
	void SwitchWeapon();
	void ToggleWeapon();
	bool IsAimDownSight() const { return bAimDownSight; }
	
	// USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }
	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable)
	APlayerCharacterController* GetPlayerCharacterController() const { return PlayerCharacterController.Get(); }


private:
	UPROPERTY(EditAnywhere, Category ="PlayerCharacter")
	FName WeaponSocket = FName("HandGrip_R");
	
	UPROPERTY(EditAnywhere, Category ="PlayerCharacter", meta = (ClampMin = 0, ClampMax = 100000, Units = "cm"))
	float MaxAimDistance = 10000.0f;
	
	// UPROPERTY(VisibleAnywhere)
	// USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<UAnimInstance> UnarmedAnimInstanceClass;

	TWeakObjectPtr<APlayerCharacterController> PlayerCharacterController;
	TWeakObjectPtr<AWeapon> CurrentWeapon;
	bool bAimDownSight{false};
};


