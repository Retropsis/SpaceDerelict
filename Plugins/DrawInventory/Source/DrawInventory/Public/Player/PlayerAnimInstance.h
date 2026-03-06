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
	
	UFUNCTION(BlueprintCallable)
	bool IsGloveRaised() const { return bGloveRaised; }
	
	UFUNCTION(BlueprintCallable)
	void SetControlRigAlpha(float Alpha) { ControlRigAlpha = FMath::Clamp(Alpha, 0.f, 1.f); }
	
	UFUNCTION(BlueprintCallable)
	float GetControlRigAlpha() const { return ControlRigAlpha; }
	
	UFUNCTION(BlueprintCallable)
	bool ShouldControlArms() const { return bAimDownSight; }

private:
	TWeakObjectPtr<APlayerCharacterController> PlayerController;
	float Pitch;
	bool bGloveRaised;
	float ControlRigAlpha{1.f};
	bool bAimDownSight{false};
};
