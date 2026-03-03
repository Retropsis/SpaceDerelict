// Retropsis 2026

#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Equipment/Weapon/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerAnimInstance.h"
#include "Player/PlayerCharacterController.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);
	
	// FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
	// FirstPersonMesh->SetupAttachment(GetMesh());
	// FirstPersonMesh->SetOnlyOwnerSee(true);
	// FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	// FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	
	// GetMesh()->SetupAttachment(GetMesh());
	// GetMesh()->SetOnlyOwnerSee(true);
	// GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	// GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(GetMesh(), FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	// FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	// FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	// FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	// FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	PlayerCharacterController = Cast<APlayerCharacterController>(NewController);
}

void APlayerCharacter::AttachWeaponMeshes(AWeapon* Weapon)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToActor(this, AttachmentRule);
	Weapon->GetWeaponMesh()->AttachToComponent(GetMesh(), AttachmentRule, WeaponSocket);
	// Weapon->GetWeaponMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, WeaponSocket);
}

FVector APlayerCharacter::GetWeaponTargetLocation()
{
	FHitResult OutHit;

	const FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	const FVector End = Start + (GetFirstPersonCameraComponent()->GetForwardVector() * MaxAimDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);
	
	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void APlayerCharacter::AddWeapon(AWeapon* Weapon)
{
	// AWeapon* OwnedWeapon = FindWeaponOfType(WeaponClass);

	if (true /*!OwnedWeapon*/)
	{
		if (IsValid(Weapon))
		{
			// OwnedWeapons.Add(AddedWeapon);

			if (CurrentWeapon.IsValid())
			{
				CurrentWeapon->DeactivateWeapon();
			}

			CurrentWeapon = Weapon;
			CurrentWeapon->ActivateWeapon();
			bAimDownSight = true;
		}
	}
}

void APlayerCharacter::HolsterWeapon(AWeapon* Weapon)
{
	if (IsValid(Weapon))
	{
		if (CurrentWeapon.IsValid())
		{
			CurrentWeapon->DeactivateWeapon();
			bAimDownSight = false;
		}
	}
}

void APlayerCharacter::OnWeaponActivated(AWeapon* Weapon)
{
	// update the bullet counter
	// OnBulletCountUpdated.Broadcast(Weapon->GetMagazineSize(), Weapon->GetBulletCount());

	// set the character mesh AnimInstances
	// GetFirstPersonMesh()->SetAnimInstanceClass(Weapon->GetFirstPersonAnimInstanceClass());
	// GetMesh()->SetAnimInstanceClass(Weapon->GetThirdPersonAnimInstanceClass());
	GetMesh()->SetAnimInstanceClass(Weapon->GetFirstPersonAnimInstanceClass());
}

void APlayerCharacter::OnWeaponDeactivated(AWeapon* Weapon)
{
	GetMesh()->SetAnimInstanceClass(UnarmedAnimInstanceClass);
}

void APlayerCharacter::StartFiring() const
{
	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon->StartFiring();
	}
}

void APlayerCharacter::StopFiring() const
{
	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon->StopFiring();
	}
}

void APlayerCharacter::SwitchWeapon()
{
}

void APlayerCharacter::ToggleWeapon()
{
	if (CurrentWeapon.IsValid())
	{
		if (bAimDownSight)
		{
			CurrentWeapon->DeactivateWeapon();
		}
		else
		{
			CurrentWeapon->ActivateWeapon();
		}
		bAimDownSight = !bAimDownSight;
	}
}

void APlayerCharacter::OnSemiWeaponRefire()
{
	// 
}

