#pragma once

#include "GridTypes.generated.h"

class UInventoryItem;

UENUM(BlueprintType)
enum class EGridCategory : uint8
{
	Item,
	Room,
	None
};

USTRUCT()
struct FSlotAvailability
{
	GENERATED_BODY()

	FSlotAvailability() {}
	FSlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {}

	int32 Index{INDEX_NONE};
	int32 AmountToFill{0};
	bool bItemAtIndex{false};
};

USTRUCT()
struct FSlotAvailabilityResult
{
	GENERATED_BODY()

	FSlotAvailabilityResult() {}

	TWeakObjectPtr<UInventoryItem> Item;

	int32 TotalRoomToFill{0};
	int32 Remainder{0};
	bool bStackable{false};
	TArray<FSlotAvailability> SlotAvailabilities;
};

UENUM(BlueprintType)
enum class ETileQuadrant : uint8
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

USTRUCT(BlueprintType)
struct FTileParameters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory")
	FIntPoint Coordinates{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory")
	int32 Index{INDEX_NONE};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory")
	ETileQuadrant Quadrant{ETileQuadrant::None};
};

inline bool operator==(const FTileParameters& A, const FTileParameters& B)
{
	return A.Coordinates == B.Coordinates && A.Index == B.Index && A.Quadrant == B.Quadrant;
}

USTRUCT()
struct FSpaceQueryResult
{
	GENERATED_BODY()

	// True if space queried has no item in it
	bool bHasSpace{false};

	// Valid if there's an item we can swap with
	TWeakObjectPtr<UInventoryItem> ValidItem = nullptr;

	// Upper Left Index of this valid item
	int32 UpperLeftIndex{INDEX_NONE};
};
