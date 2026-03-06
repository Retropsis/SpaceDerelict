// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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
	virtual void AttachEquipmentMeshes(AActiveEquipActor* ActiveEquipment) override;
	virtual FVector GetActiveTargetLocation() override;
	virtual void OnSemiWeaponRefire() override;
	virtual void AddActiveEquipment(AActiveEquipActor* ActiveEquipment) override;
	virtual void HolsterActiveEquipment(AActiveEquipActor* ActiveEquipment) override;
	virtual void OnActiveEquipmentActivated(AActiveEquipActor* ActiveEquipment) override;
	virtual void OnActiveEquipmentDeactivated(AActiveEquipActor* ActiveEquipment) override;
	//~ WeaponInterface
	
	void StartActive() const;
	void StopActive() const;
	void SwitchActive();
	void ToggleActive(const FGameplayTag& EquipmentType);
	
	// USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }
	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable)
	APlayerCharacterController* GetPlayerCharacterController() const { return PlayerCharacterController.Get(); }


private:
	UPROPERTY(EditAnywhere, Category ="PlayerCharacter")
	FName ActiveSocket = FName("HandGrip_R");
	
	UPROPERTY(EditAnywhere, Category ="PlayerCharacter", meta = (ClampMin = 0, ClampMax = 100000, Units = "cm"))
	float MaxAimDistance = 10000.0f;
	
	// UPROPERTY(VisibleAnywhere)
	// USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<UAnimInstance> UnarmedAnimInstanceClass;

	TWeakObjectPtr<APlayerCharacterController> PlayerCharacterController;
	TWeakObjectPtr<AActiveEquipActor> CurrentActive;
	TMap<FGameplayTag, AActiveEquipActor*> OwnedActives;
};


