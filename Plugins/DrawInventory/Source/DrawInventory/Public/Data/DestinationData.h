#pragma once

#include "GameplayTagContainer.h"
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
	FDestinationAvailability(int32 Index, FGameplayTag NewLayer, EDoorState State, FName SocketToAttach) : DestinationIndex(Index), Layer(NewLayer), DoorState(State), Socket(SocketToAttach) {}

	int32 DestinationIndex{INDEX_NONE};
	FGameplayTag Layer = FGameplayTag::EmptyTag;
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
	FGameplayTag Layer = FGameplayTag::EmptyTag;
	TArray<FDestinationAvailability> DestinationAvailabilities;
};