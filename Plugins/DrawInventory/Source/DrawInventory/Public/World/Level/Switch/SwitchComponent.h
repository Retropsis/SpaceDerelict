// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionComponent.h"
#include "SwitchComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwitch, bool, bOn);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API USwitchComponent : public UInteractionComponent
{
	GENERATED_BODY()

public:
	USwitchComponent();
	virtual void Interact() override;

	UPROPERTY(BlueprintAssignable, Category="DrawInventory")
	FSwitch OnSwitch;

private:
	void ToggleSwitch(bool bOn);
	
	bool bIsOn{false};
};
