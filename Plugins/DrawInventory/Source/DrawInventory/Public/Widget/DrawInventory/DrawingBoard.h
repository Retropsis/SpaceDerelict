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
class UDrawingPanel;
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
	FDestinationAvailabilityResult HasRoom(FItemManifest& Manifest, int32 RoomIndex, int32 DestinationIndex, int32 DestinationYaw, const TSet<FGameplayTag>& Layers, const FGameplayTag& Layer) const;
	bool HasRoomAtIndex(const int32 Index, const TSet<FGameplayTag>& Layers) const;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayOpeningVisualEffects();

private:
	UFUNCTION()
	void OnPlayerLayerUpdate(const FGameplayTag& Layer);
	
	UFUNCTION()
	void ShowLowerLayerGrid();
	
	UFUNCTION()
	void ShowMiddleLayerGrid();
	
	UFUNCTION()
	void ShowUpperLayerGrid();
	
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

	TWeakObjectPtr<UDrawComponent> DrawComponent;
	TWeakObjectPtr<UDrawingGrid> ActiveGrid;
};
