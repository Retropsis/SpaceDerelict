// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

enum class EDoorState : uint8;

UCLASS()
class DRAWINVENTORY_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	ADoor();

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleDoor(bool bOpen);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SwitchDoorState(EDoorState DoorState);
};
