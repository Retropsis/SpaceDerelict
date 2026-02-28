// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/DestinationData.h"
#include "Item/Fragment/ItemFragment.h"
#include "DrawingBoard.generated.h"

struct FItemManifest;
struct FImageFragment;
struct FGridFragment;
class UWidgetSwitcher;
class UTextBlock;
class UButton;
class UDrawingGrid;
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
	void DrawRoom(UInventoryItem* Room, int32 Index, int32 Yaw, const FGameplayTag& Layer, bool bRequirementMet);
	FDestinationAvailabilityResult HasRoom(FItemManifest& Manifest, int32 RoomIndex, int32 DestinationIndex, int32 DestinationYaw, const TSet<FGameplayTag>&
	                                       Layers, const FGameplayTag& Layer) const;
	void SetRedrawCount(int32 Count) const;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayOpeningVisualEffects();

private:
	UFUNCTION()
	void OnDrawnRoomSlotClicked(UDrawnRoomSlot* DrawnRoomSlot);
	
	UFUNCTION()
	void OnDrawnRoomSlotHovered(UDrawnRoomSlot* DrawnRoomSlot);
	
	UFUNCTION()
	void OnDrawnRoomSlotUnhovered(UDrawnRoomSlot* DrawnRoomSlot);
	
	UFUNCTION()
	void OnRedrawButtonClicked();

	UFUNCTION()
	void OnPlayerLayerUpdate(const FGameplayTag& Layer);
	
	UFUNCTION()
	void ShowLowerLayerGrid();
	
	UFUNCTION()
	void ShowMiddleLayerGrid();
	
	UFUNCTION()
	void ShowUpperLayerGrid();
	
	void SetDrawnRoomSlotImage(const UDrawnRoomSlot* DrawnRoomSlot, const FGridFragment* GridFragment, const FImageFragment* ImageFragment) const;
	FVector2D GetDrawSize(const FGridFragment* GridFragment) const;
	void SetActiveGrid(UDrawingGrid* Grid, UButton* Button);
	void DisableButton(UButton* Button);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDrawingGrid> Grid_Upper;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDrawingGrid> Grid_Middle;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDrawingGrid> Grid_Lower;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher_Layers;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_LowerLayer;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_MiddleLayer;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_UpperLayer;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_DrawnRoomSlots;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Redraw;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_RedrawCount;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_RedrawBox;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<UDrawnRoomSlot> DrawnRoomSlotClass;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	float TileSize{240};
	
	UPROPERTY()
	TArray<TObjectPtr<UDrawnRoomSlot>> DrawnRoomSlots;

	TWeakObjectPtr<UDrawComponent> DrawComponent;
	TWeakObjectPtr<UDrawingGrid> ActiveGrid;
};
