// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/DestinationData.h"
#include "Item/Fragment/ItemFragment.h"
#include "DrawingBoard.generated.h"

struct FItemManifest;
class UDrawingGrid;
struct FImageFragment;
struct FGridFragment;
class UHorizontalBox;
class UInventoryItem;
class UDrawComponent;
class UDrawnRoomSlot;


UCLASS()
class DRAWINVENTORY_API UDrawingBoard : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	void ClearDrawingBoard();
	void DrawRoom(UInventoryItem* Room, int32 Index, int32 Yaw, bool bRequirementMet);
	FDestinationAvailabilityResult HasRoom(const FItemManifest& Manifest, int32 RoomIndex, int32 DestinationIndex, int32 DestinationYaw) const;

private:
	UFUNCTION()
	void OnDrawnRoomSlotClicked(UDrawnRoomSlot* DrawnRoomSlot);
	
	UFUNCTION()
	void OnDrawnRoomSlotHovered(UDrawnRoomSlot* DrawnRoomSlot);
	
	UFUNCTION()
	void OnDrawnRoomSlotUnhovered(UDrawnRoomSlot* DrawnRoomSlot);

	void SetDrawnRoomSlotImage(const UDrawnRoomSlot* DrawnRoomSlot, const FGridFragment* GridFragment, const FImageFragment* ImageFragment) const;
	FVector2D GetDrawSize(const FGridFragment* GridFragment) const;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDrawingGrid> Grid_Room;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_DrawnRoomSlots;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<UDrawnRoomSlot> DrawnRoomSlotClass;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	float TileSize{240};
	
	UPROPERTY()
	TArray<TObjectPtr<UDrawnRoomSlot>> DrawnRoomSlots;

	TWeakObjectPtr<UDrawComponent> DrawComponent;
};
