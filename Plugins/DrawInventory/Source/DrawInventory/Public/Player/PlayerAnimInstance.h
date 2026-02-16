// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacterController;

UCLASS()
class DRAWINVENTORY_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	float GetPitch() const { return Pitch; }

private:
	TWeakObjectPtr<APlayerCharacterController> PlayerController;
	float Pitch;
};
