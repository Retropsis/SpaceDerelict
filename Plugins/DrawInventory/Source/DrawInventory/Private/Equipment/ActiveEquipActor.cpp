// Retropsis 2026

#include "Equipment/ActiveEquipActor.h"
#include "Equipment/Weapon/WeaponInterface.h"

AActiveEquipActor::AActiveEquipActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	EquipmentMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
	EquipmentMesh->SetupAttachment(RootComponent);

	EquipmentMesh->SetCollisionProfileName(FName("NoCollision"));
	EquipmentMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);
	// WeaponMesh->bOnlyOwnerSee = true;
}

void AActiveEquipActor::InitializeEquipment()
{
	if (const AController* OwnerController = Cast<AController>(GetOwner()))
	{		
		if (APawn* OwnerPawn = Cast<APawn>(OwnerController->GetPawn()))
		{
			EquipmentOwner = Cast<IWeaponInterface>(OwnerPawn);
			PawnOwner = Cast<APawn>(OwnerPawn);
		}
	}
}

void AActiveEquipActor::OnEquip(APawn* Pawn)
{
	if (IWeaponInterface* WeaponHolder = Cast<IWeaponInterface>(Pawn))
	{
		WeaponHolder->AddActiveEquipment(this);
	}
}

void AActiveEquipActor::OnUnequip(APawn* Pawn)
{
	if (IWeaponInterface* WeaponHolder = Cast<IWeaponInterface>(Pawn))
	{
		WeaponHolder->HolsterActiveEquipment(this);
	}
}

void AActiveEquipActor::ActivateEquipment()
{
	SetActorHiddenInGame(false);
	EquipmentOwner->OnActiveEquipmentActivated(this);
}

void AActiveEquipActor::DeactivateEquipment()
{
	StopActive();
	SetActorHiddenInGame(true);
	EquipmentOwner->OnActiveEquipmentDeactivated(this);
}

void AActiveEquipActor::StartActive()
{
	UE_LOG(LogTemp, Warning, TEXT("StartActive"));
}

void AActiveEquipActor::StopActive()
{
	UE_LOG(LogTemp, Warning, TEXT("StopActive"));
}

