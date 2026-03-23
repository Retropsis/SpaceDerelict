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

void APlayerCharacter::AttachEquipmentMeshes(AActiveEquipActor* ActiveEquipment)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
	ActiveEquipment->AttachToActor(this, AttachmentRule);
	ActiveEquipment->GetEquipmentMesh()->AttachToComponent(GetMesh(), AttachmentRule, ActiveSocket);
	// Weapon->GetWeaponMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, WeaponSocket);
}

FVector APlayerCharacter::GetActiveTargetLocation()
{
	FHitResult OutHit;

	const FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	const FVector End = Start + (GetFirstPersonCameraComponent()->GetForwardVector() * MaxAimDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);
	
	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void APlayerCharacter::AddActiveEquipment(AActiveEquipActor* ActiveEquipment)
{
	// AWeapon* OwnedWeapon = FindWeaponOfType(WeaponClass);

	if (true /*!OwnedWeapon*/)
	{
		if (IsValid(ActiveEquipment))
		{
			UE_LOG(LogTemp, Warning, TEXT("Adding Weapon of Type: %s"), *ActiveEquipment->GetEquipmentType().ToString());
			OwnedActives.Add(ActiveEquipment->GetEquipmentType(), ActiveEquipment);

			if (CurrentActive.IsValid())
			{
				CurrentActive->DeactivateEquipment();
			}

			CurrentActive = ActiveEquipment;
			CurrentActive->ActivateEquipment();
		}
	}
}

void APlayerCharacter::HolsterActiveEquipment(AActiveEquipActor* ActiveEquipment)
{
	if (IsValid(ActiveEquipment))
	{
		if (CurrentActive.IsValid())
		{
			CurrentActive->DeactivateEquipment();
		}
	}
}

void APlayerCharacter::OnActiveEquipmentActivated(AActiveEquipActor* ActiveEquipment)
{
	// update the bullet counter
	// OnBulletCountUpdated.Broadcast(Weapon->GetMagazineSize(), Weapon->GetBulletCount());

	// set the character mesh AnimInstances
	// GetFirstPersonMesh()->SetAnimInstanceClass(Weapon->GetFirstPersonAnimInstanceClass());
	// GetMesh()->SetAnimInstanceClass(Weapon->GetThirdPersonAnimInstanceClass());
	GetMesh()->SetAnimInstanceClass(ActiveEquipment->GetFirstPersonAnimInstanceClass());
}

void APlayerCharacter::OnActiveEquipmentDeactivated(AActiveEquipActor* ActiveEquipment)
{
	GetMesh()->SetAnimInstanceClass(UnarmedAnimInstanceClass);
}

FGameplayTag APlayerCharacter::GetActiveEquipmentTag()
{
	if (CurrentActive.IsValid())
	{
		return CurrentActive->GetEquipmentType();
	}
	return  FGameplayTag::EmptyTag;
}

void APlayerCharacter::StartActive() const
{
	if (CurrentActive.IsValid())
	{
		CurrentActive->StartActive();
	}
}

void APlayerCharacter::StopActive() const
{
	if (CurrentActive.IsValid())
	{
		CurrentActive->StopActive();
	}
}

void APlayerCharacter::SwitchActive()
{
}

void APlayerCharacter::ToggleActive(const FGameplayTag& EquipmentType)
{
	if (!OwnedActives.Contains(EquipmentType)) return;
	
	if (CurrentActive.IsValid())
	{
		CurrentActive->DeactivateEquipment();

		CurrentActive = OwnedActives[EquipmentType];
		
		CurrentActive->ActivateEquipment();
	}
}

void APlayerCharacter::OnSemiWeaponRefire()
{
	// 
}

