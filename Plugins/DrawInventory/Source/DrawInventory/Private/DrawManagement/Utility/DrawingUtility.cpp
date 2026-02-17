// Retropsis 2026

#include "DrawManagement/Utility/DrawingUtility.h"
#include "DrawManagement/Component/DrawComponent.h"

UDrawComponent* UDrawingUtility::GetDrawComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;

	UDrawComponent* DrawComponent = PlayerController->FindComponentByClass<UDrawComponent>();
	return DrawComponent;
}

FIntPoint UDrawingUtility::GetOffsetFromSocketName(const FName& Socket)
{
	if (Socket == "South") return FIntPoint(0, -1);
	if (Socket == "West") return FIntPoint(-1, 0);
	if (Socket == "North") return FIntPoint(0, 1);
	if (Socket == "East") return FIntPoint(1, 0);
	return FIntPoint(0, 0);
}

FIntPoint UDrawingUtility::GetShiftedOffsetFromAngle(const FIntPoint& Offset, int32 Angle)
{
	switch (Angle)
	{
	case 0: return Offset;
	case 90:
		if (Offset == FIntPoint(-1, 0 )) return FIntPoint(0, 1);
		if (Offset == FIntPoint(1, 0 )) return FIntPoint(0, -1);
		if (Offset == FIntPoint(0, -1 )) return FIntPoint(-1, 0);
		if (Offset == FIntPoint(0, 1 )) return FIntPoint(1, 0);
	case -180:
	case 180:
		if (Offset == FIntPoint(-1, 0 )) return FIntPoint(1, 0);
		if (Offset == FIntPoint(1, 0 )) return FIntPoint(-1, 0);
		if (Offset == FIntPoint(0, -1 )) return FIntPoint(0, 1);
		if (Offset == FIntPoint(0, 1 )) return FIntPoint(0, -1);
	case -90:
		if (Offset == FIntPoint(-1, 0 )) return FIntPoint(0, -1);
		if (Offset == FIntPoint(1, 0 )) return FIntPoint(0, 1);
		if (Offset == FIntPoint(0, -1 )) return FIntPoint(1, 0);
		if (Offset == FIntPoint(0, 1 )) return FIntPoint(-1, 0);
	default: return Offset;
	}
}

FName UDrawingUtility::GetSocketNameFromOffset(const FIntPoint& Offset)
{
	if (Offset == FIntPoint(-1, 0 )) return FName("West");
	if (Offset == FIntPoint(1, 0 )) return FName("East");
	if (Offset == FIntPoint(0, -1 )) return FName("South");
	if (Offset == FIntPoint(0, 1 )) return FName("North");
	return FName("None");
}

FName UDrawingUtility::FindConnectedDoorSocket(const FIntPoint& OriginOffset, const int32 Yaw)
{
	switch (Yaw)
	{
	case 0:
		if (OriginOffset == FIntPoint(-1, 0 )) return FName("East");
		if (OriginOffset == FIntPoint(1, 0 )) return FName("West");
		if (OriginOffset == FIntPoint(0, -1 )) return FName("North");
		if (OriginOffset == FIntPoint(0, 1 )) return FName("South");
	case 90: 
		if (OriginOffset == FIntPoint(-1, 0 )) return FName("North");
		if (OriginOffset == FIntPoint(1, 0 )) return FName("South");
		if (OriginOffset == FIntPoint(0, -1 )) return FName("West");
		if (OriginOffset == FIntPoint(0, 1 )) return FName("East");
	case -180: 
	case 180:
		if (OriginOffset == FIntPoint(-1, 0 )) return FName("West");
		if (OriginOffset == FIntPoint(1, 0 )) return FName("East");
		if (OriginOffset == FIntPoint(0, -1 )) return FName("South");
		if (OriginOffset == FIntPoint(0, 1 )) return FName("North");
	case -90: 
		if (OriginOffset == FIntPoint(-1, 0 )) return FName("South");
		if (OriginOffset == FIntPoint(1, 0 )) return FName("North");
		if (OriginOffset == FIntPoint(0, -1 )) return FName("East");
		if (OriginOffset == FIntPoint(0, 1 )) return FName("West");
		default: return FName("None");
	}
}
