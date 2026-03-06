// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "EquipmentManagement/EquipActor/EquipActor.h"
#include "ActiveEquipActor.generated.h"

class IWeaponInterface;

UCLASS()
class DRAWINVENTORY_API AActiveEquipActor : public AEquipActor
{
	GENERATED_BODY()

public:
	AActiveEquipActor();
	virtual void InitializeEquipment() override;
	virtual void OnEquip(APawn* Pawn) override;
	virtual void OnUnequip(APawn* Pawn) override;
	virtual void ActivateEquipment();
	virtual void DeactivateEquipment();
	virtual void StartActive();
	virtual void StopActive();
	virtual const TSubclassOf<UAnimInstance>& GetFirstPersonAnimInstanceClass() const { return FirstPersonAnimInstanceClass; }
	virtual USkeletalMeshComponent* GetEquipmentMesh() const { return EquipmentMesh; }

protected:
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	USkeletalMeshComponent* EquipmentMesh;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<UAnimInstance> FirstPersonAnimInstanceClass;
	
	UPROPERTY()
	TObjectPtr<APawn> PawnOwner;
	
	IWeaponInterface* EquipmentOwner = nullptr;
};
