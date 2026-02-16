#pragma once

#include "DestinationData.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Opened,
	Closed,
	Locked,
	Sealed,
	None
};

USTRUCT()
struct FDestinationAvailability
{
	GENERATED_BODY()

	FDestinationAvailability() {}
	FDestinationAvailability(int32 Index, EDoorState State, FName SocketToAttach) : DestinationIndex(Index), DoorState(State), Socket(SocketToAttach) {}

	int32 DestinationIndex{INDEX_NONE};
	EDoorState DoorState{EDoorState::None};
	FName Socket = NAME_None;
};

USTRUCT()
struct FDestinationAvailabilityResult
{
	GENERATED_BODY()

	FDestinationAvailabilityResult() {}

	// TWeakObjectPtr<UInventoryItem> Item;

	int32 RoomIndex{INDEX_NONE};
	int32 DestinationYaw{ INDEX_NONE };
	TArray<FDestinationAvailability> DestinationAvailabilities;
};