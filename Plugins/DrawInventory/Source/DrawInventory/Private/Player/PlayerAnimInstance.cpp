// Retropsis 2026


#include "Player/PlayerAnimInstance.h"

#include "GameFramework/Character.h"
#include "Player/PlayerCharacterController.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (ACharacter* Character = Cast<ACharacter>(GetOwningActor()))
	{
		PlayerController = Cast<APlayerCharacterController>(Character->GetController());
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!PlayerController.IsValid())
	{
		if (ACharacter* Character = Cast<ACharacter>(GetOwningActor()))
		{
			PlayerController = Cast<APlayerCharacterController>(Character->GetController());
		}
	}
	if (!PlayerController.IsValid()) return;

	Pitch = PlayerController->GetAOPitch();
}
