// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DrawingUtility.generated.h"

class UDrawComponent;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API UDrawingUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "DrawInventory")
	static UDrawComponent* GetDrawComponent(const APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable, Category = "DrawInventory")
	static FIntPoint GetOffsetFromSocketName(const FName& Socket);
	
	UFUNCTION(BlueprintCallable, Category = "DrawInventory")
	static FIntPoint GetShiftedOffsetFromAngle(const FIntPoint& Offset, int32 Angle);
	
	UFUNCTION(BlueprintCallable, Category = "DrawInventory")
	static FName GetSocketNameFromOffset(const FIntPoint& Offset);
	
	UFUNCTION(BlueprintCallable, Category = "DrawInventory")
	static FName FindConnectedDoorSocket(const FIntPoint& OriginOffset, int32 Yaw);
};
