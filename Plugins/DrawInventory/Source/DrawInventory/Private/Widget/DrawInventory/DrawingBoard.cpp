// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/DrawInventory/DrawingBoard.h"
#include "Components/Button.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/WidgetSwitcher.h"
#include "DrawManagement/Component/DrawComponent.h"
#include "DrawManagement/Utility/DrawingUtility.h"
#include "Item/Fragment/FragmentTags.h"
#include "Player/PlayerCharacterController.h"
#include "Widget/DrawInventory/DrawingGrid.h"

void UDrawingBoard::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	DrawComponent = UDrawingUtility::GetDrawComponent(GetOwningPlayer());
	
	ShowMiddleLayerGrid();
	
	Button_LowerLayer->OnClicked.AddDynamic(this, &ThisClass::ShowLowerLayerGrid);
	Button_MiddleLayer->OnClicked.AddDynamic(this, &ThisClass::ShowMiddleLayerGrid);
	Button_UpperLayer->OnClicked.AddDynamic(this, &ThisClass::ShowUpperLayerGrid);

	// Grid_Lower->SetOwningCanvas(CanvasPanel);
	// Grid_Middle->SetOwningCanvas(CanvasPanel);
	// Grid_Upper->SetOwningCanvas(CanvasPanel);
	
	if (APlayerCharacterController* PC = Cast<APlayerCharacterController>(GetOwningPlayer()))
	{
		PC->OnPlayerLayerUpdated.AddDynamic(this, &ThisClass::OnPlayerLayerUpdate);
	}
}

FDestinationAvailabilityResult UDrawingBoard::HasRoom(FItemManifest& Manifest, const int32 RoomIndex, const int32 DestinationIndex, const int32 DestinationYaw, const TSet<FGameplayTag>& Layers, const FGameplayTag& Layer) const
{
	FDestinationAvailabilityResult Result;
	Result.RoomIndex = DestinationIndex;
	Result.DestinationYaw = DestinationYaw;
	Result.Layer = Layer;
	
	if (Layers.Contains(Layer::Lower)) Grid_Lower->HasRoom(Manifest, RoomIndex, DestinationIndex, DestinationYaw, Result);
	if (Layers.Contains(Layer::Middle)) Grid_Middle->HasRoom(Manifest, RoomIndex, DestinationIndex, DestinationYaw, Result);
	if (Layers.Contains(Layer::Upper)) Grid_Upper->HasRoom(Manifest, RoomIndex, DestinationIndex, DestinationYaw, Result);
	
	return Result;
}

bool UDrawingBoard::HasRoomAtIndex(const int32 Index, const TSet<FGameplayTag>& Layers) const
{
	for (const FGameplayTag& Layer : Layers)
	{
		if (Layer.MatchesTagExact(Layer::Lower) && !Grid_Lower->HasRoomAtIndex(Index))  return false;
		if (Layer.MatchesTagExact(Layer::Middle) && !Grid_Middle->HasRoomAtIndex(Index)) return false;
		if (Layer.MatchesTagExact(Layer::Upper) && !Grid_Upper->HasRoomAtIndex(Index)) return false;
	}
	return true;
}

void UDrawingBoard::OnPlayerLayerUpdate(const FGameplayTag& Layer)
{
	if (Layer.MatchesTagExact(Layer::Lower)) Switcher_Layers->SetActiveWidget(Grid_Lower);
	if (Layer.MatchesTagExact(Layer::Middle)) Switcher_Layers->SetActiveWidget(Grid_Middle);
	if (Layer.MatchesTagExact(Layer::Upper)) Switcher_Layers->SetActiveWidget(Grid_Upper);
}

void UDrawingBoard::ShowLowerLayerGrid()
{
	SetActiveGrid(Grid_Lower, Button_LowerLayer);	
}

void UDrawingBoard::ShowMiddleLayerGrid()
{
	SetActiveGrid(Grid_Middle, Button_MiddleLayer);	
}

void UDrawingBoard::ShowUpperLayerGrid()
{
	SetActiveGrid(Grid_Upper, Button_UpperLayer);	
}

void UDrawingBoard::SetActiveGrid(UDrawingGrid* Grid, UButton* Button)
{
	if (ActiveGrid.IsValid())
	{
		// ActiveGrid->HideCursor();
		// ActiveGrid->OnHide();
	}
	ActiveGrid = Grid;
	// if (ActiveGrid.IsValid()) ActiveGrid->ShowCursor();
	DisableButton(Button);
	Switcher_Layers->SetActiveWidget(Grid);
}

void UDrawingBoard::DisableButton(UButton* Button)
{
	Button_LowerLayer->SetIsEnabled(true);
	Button_MiddleLayer->SetIsEnabled(true);
	Button_UpperLayer->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}
