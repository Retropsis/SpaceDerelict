// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Data/DestinationData.h"
#include "Interaction/InteractionComponent.h"
#include "DoorComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UDoorComponent : public UInteractionComponent
{
	GENERATED_BODY()

public:
	UDoorComponent();
	void ToggleDoor(bool bOpen);
	bool IsLocked() const { return DoorState == EDoorState::Locked; }
	void Unlock();
	void SwitchDoorState(const EDoorState NewState);
	void SetDoorState(const EDoorState NewState) { DoorState = NewState; }
	bool IsSealed() const { return DoorState == EDoorState::Sealed; }
	void SetRoomIndex(int32 Index) { RoomIndex = Index; }
	int32 GetRoomIndex() const { return RoomIndex; }
	void SetDestinationIndex(int32 Index) { DestinationIndex = Index; }
	int32 GetDestinationIndex() const { return DestinationIndex; }
	void SetRoomYaw(int32 NewYaw) { Yaw = NewYaw; }
	float GetRoomYaw() const { return Yaw; }
	void SetInteractionMessageToLocked();

private:
	EDoorState DoorState;
	int32 RoomIndex{ INDEX_NONE };
	int32 DestinationIndex{ INDEX_NONE };
	int32 Yaw{ INDEX_NONE };
};
