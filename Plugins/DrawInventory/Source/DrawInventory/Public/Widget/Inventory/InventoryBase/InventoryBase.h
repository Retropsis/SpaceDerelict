// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/GridTypes.h"
#include "InventoryBase.generated.h"

class UHoverItem;
class UItemComponent;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API UInventoryBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FSlotAvailabilityResult HasRoomForItem(UItemComponent* ItemComponent) { return FSlotAvailabilityResult(); }
	virtual void OnItemHovered(UInventoryItem* Item) {}
	virtual void OnItemUnhovered() {}
	virtual bool HasHoverItem() const { return false; }
	virtual UHoverItem* GetHoverItem() const { return nullptr; }
	virtual float GetTileSize() const { return 0.0f; }
};
