// Retropsis 2026

#include "Equipment/Weapon/Weapon.h"
#include "Equipment/Weapon/Projectile.h"
#include "Equipment/Weapon/WeaponInterface.h"
#include "GameFramework/Character.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "Item/ItemTags.h"
#include "Kismet/KismetMathLibrary.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponMesh->SetCollisionProfileName(FName("NoCollision"));
	WeaponMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);
	WeaponMesh->bOnlyOwnerSee = true;
}

void AWeapon::InitializeEquipment()
{
	// GetOwner()->OnDestroyed.AddDynamic(this, &AWeapon::OnOwnerDestroyed);
	if (const AController* OwnerController = Cast<AController>(GetOwner()))
	{
		InventoryComponent = OwnerController->FindComponentByClass<UInventoryComponent>();
		InventoryComponent->OnItemOfTypeStackChange.AddDynamic(this, &ThisClass::OnBulletCountChange);
		CurrentBullets = InventoryComponent->GetItemOfTypeAmount(Item::Equipment::Ammunition::Bullet);
		
		if (APawn* OwnerPawn = Cast<APawn>(OwnerController->GetPawn()))
		{
			WeaponOwner = Cast<IWeaponInterface>(OwnerPawn);
			PawnOwner = Cast<APawn>(OwnerPawn);
			// CurrentBullets = MagazineSize;
			// WeaponOwner->AttachWeaponMeshes(this);
		}
	}
}

void AWeapon::OnBulletCountChange(const FGameplayTag& ItemType, int32 Amount)
{
	if (ItemType.MatchesTagExact(Item::Equipment::Ammunition::Bullet))
	{
		CurrentBullets = Amount;
	}
}

void AWeapon::OnEquip(APawn* Pawn)
{
	if (IWeaponInterface* WeaponHolder = Cast<IWeaponInterface>(Pawn))
	{
		WeaponHolder->AddWeapon(this);
	}
}

void AWeapon::ActivateWeapon()
{
	SetActorHiddenInGame(false);
	WeaponOwner->OnWeaponActivated(this);
}

void AWeapon::DeactivateWeapon()
{
	SetActorHiddenInGame(true);
	WeaponOwner->OnWeaponDeactivated(this);
}

void AWeapon::StartFiring()
{
	if (CurrentBullets <= 0) return;
	
	bIsFiring = true;

	const float TimeSinceLastShot = GetWorld()->GetTimeSeconds() - TimeOfLastShot;
	if (TimeSinceLastShot > FireRate)
	{
		Fire();
	}
	else
	{
		if (bFullAuto)
		{
			GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AWeapon::Fire, TimeSinceLastShot, false);
		}
	}
}

void AWeapon::StopFiring()
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
}

void AWeapon::Fire()
{
	if (!bIsFiring)
	{
		return;
	}
	
	FireProjectile(WeaponOwner->GetWeaponTargetLocation());
	TimeOfLastShot = GetWorld()->GetTimeSeconds();

	// MakeNoise(ShotLoudness, PawnOwner, PawnOwner->GetActorLocation(), ShotNoiseRange, ShotNoiseTag);

	if (bFullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AWeapon::Fire, FireRate, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AWeapon::FireCooldownExpired, FireRate, false);
	}
}

void AWeapon::FireProjectile(const FVector& TargetLocation)
{
	FTransform ProjectileTransform = CalculateProjectileSpawnTransform(TargetLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = PawnOwner;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileTransform, SpawnParams);

	if (InventoryComponent.IsValid())
	{
		InventoryComponent->Server_ConsumeItemOfTypAndAmount(Item::Equipment::Ammunition::Bullet, 1);
	}

	// play the firing montage
	// WeaponOwner->PlayFiringMontage(FiringMontage);
	//
	// // add recoil
	// WeaponOwner->AddWeaponRecoil(FiringRecoil);
	//
	// // consume bullets
	// --CurrentBullets;
	//
	// // if the clip is depleted, reload it
	// if (CurrentBullets <= 0)
	// {
	// 	CurrentBullets = MagazineSize;
	// }
	//
	// // update the weapon HUD
	// WeaponOwner->UpdateWeaponHUD(CurrentBullets, MagazineSize);
}

void AWeapon::FireCooldownExpired()
{
	WeaponOwner->OnSemiWeaponRefire();
}

FTransform AWeapon::CalculateProjectileSpawnTransform(const FVector& TargetLocation) const
{
	const FVector MuzzleLoc = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	const FVector SpawnLoc = MuzzleLoc + ((TargetLocation - MuzzleLoc).GetSafeNormal() * MuzzleOffset);
	const FRotator AimRot = UKismetMathLibrary::FindLookAtRotation(SpawnLoc, TargetLocation + (UKismetMathLibrary::RandomUnitVector() * AimVariance));
	return FTransform(AimRot, SpawnLoc, FVector::OneVector);
}
