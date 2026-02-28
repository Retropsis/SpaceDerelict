// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/DrawInventory/DrawingBoard.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/WidgetSwitcher.h"
#include "DrawManagement/Component/DrawComponent.h"
#include "DrawManagement/Utility/DrawingUtility.h"
#include "Item/InventoryItem.h"
#include "Item/Fragment/FragmentTags.h"
#include "Item/Fragment/ItemFragment.h"
#include "Player/PlayerCharacterController.h"
#include "Widget/DrawInventory/DrawingGrid.h"
#include "Widget/DrawInventory/SlottedRoom/DrawnRoomSlot.h"

void UDrawingBoard::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	DrawComponent = UDrawingUtility::GetDrawComponent(GetOwningPlayer());
	Button_Redraw->OnClicked.AddDynamic(this, &ThisClass::UDrawingBoard::OnRedrawButtonClicked);
	
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

void UDrawingBoard::ClearDrawingBoard()
{
	HorizontalBox_DrawnRoomSlots->ClearChildren();
	DrawnRoomSlots.Empty();
}

void UDrawingBoard::DrawRoom(UInventoryItem* Room, int32 Index, int32 Yaw, const FGameplayTag& Layer, bool bRequirementMet)
{
	const FImageFragment* ImageFragment = Room->GetItemManifest().GetFragmentOfType<FImageFragment>();
	const FTextFragment* TextFragment = Room->GetItemManifest().GetFragmentOfType<FTextFragment>();
	const FGridFragment* GridFragment = Room->GetItemManifest().GetFragmentOfType<FGridFragment>();
	FRoomFragment* RoomFragment = Room->GetItemManifestMutable().GetFragmentOfTypeMutable<FRoomFragment>();
	if (!ImageFragment || !TextFragment || !GridFragment || !RoomFragment) return;

	RoomFragment->SetYaw(Yaw);
		
	UDrawnRoomSlot* DrawnRoomSlot = CreateWidget<UDrawnRoomSlot>(GetOwningPlayer(), DrawnRoomSlotClass);
	HorizontalBox_DrawnRoomSlots->AddChildToHorizontalBox(DrawnRoomSlot);
		
	UHorizontalBoxSlot* GridSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(DrawnRoomSlot);
	FSlateChildSize Size;
	Size.Value = TileSize;
	Size.SizeRule = ESlateSizeRule::Automatic;
	GridSlot->SetSize(Size);
	GridSlot->SetPadding(FMargin(20.f, 0.f, 20.f, 0.f));
		
	DrawnRoomSlot->SetInventoryItem(Room);
	DrawnRoomSlot->SetRoomName(TextFragment->GetText());
	DrawnRoomSlot->SetGridIndex(Index);
	DrawnRoomSlot->SetLayer(Layer);
	SetDrawnRoomSlotImage(DrawnRoomSlot, GridFragment, ImageFragment);
	DrawnRoomSlot->OnDrawnRoomSlotClicked.AddDynamic(this, &ThisClass::OnDrawnRoomSlotClicked);
	DrawnRoomSlot->OnDrawnRoomSlotHovered.AddDynamic(this, &ThisClass::OnDrawnRoomSlotHovered);
	DrawnRoomSlot->OnDrawnRoomSlotUnhovered.AddDynamic(this, &ThisClass::OnDrawnRoomSlotUnhovered);

	if (const FRequirementFragment* RequirementFragment = Room->GetItemManifest().GetFragmentOfType<FRequirementFragment>())
	{
		DrawnRoomSlot->SetRequirement(RequirementFragment->GetIcon(), RequirementFragment->GetAmount(), bRequirementMet);
	}
	if (const FValuableFragment* ValuableFragment = Room->GetItemManifest().GetFragmentOfType<FValuableFragment>())
	{
		DrawnRoomSlot->SetValuable(ValuableFragment->GetValuables());
	}
	DrawnRoomSlot->OnDrawVisualEffects();
	DrawnRoomSlots.Add(DrawnRoomSlot);
}

FDestinationAvailabilityResult UDrawingBoard::HasRoom(FItemManifest& Manifest, const int32 RoomIndex, const int32 DestinationIndex, const int32 DestinationYaw, const TSet<FGameplayTag>& Layers, const FGameplayTag& Layer) const
{
	FDestinationAvailabilityResult Result;
	Result.RoomIndex = DestinationIndex;
	Result.DestinationYaw = DestinationYaw;
	Result.Layer = Layer;
	
	if (Layers.Contains(Layer::Lower)) Grid_Lower->HasRoom(Manifest, RoomIndex, DestinationIndex, DestinationYaw, Result);
	if (Layers.Contains(Layer::Middle)) Grid_Upper->HasRoom(Manifest, RoomIndex, DestinationIndex, DestinationYaw, Result);
	if (Layers.Contains(Layer::Upper)) Grid_Middle->HasRoom(Manifest, RoomIndex, DestinationIndex, DestinationYaw, Result);
	
	return Result;
}

void UDrawingBoard::OnDrawnRoomSlotClicked(UDrawnRoomSlot* DrawnRoomSlot)
{
	for (const TObjectPtr<UDrawnRoomSlot>& RoomSlot : DrawnRoomSlots)
	{
		RoomSlot->OnDrawnRoomSlotUnhovered.RemoveAll(this);
	}
	
	check(DrawComponent.IsValid());
	UInventoryItem* Item = DrawnRoomSlot->GetInventoryItem().Get();
	if (!IsValid(Item))
	{
		UE_LOG(LogTemp, Error, TEXT("OnDrawnRoomSlotClicked: Item is invalid"));
	}
	DrawComponent->Server_DrawnRoomSlotClicked(DrawnRoomSlot->GetInventoryItem().Get());
}

void UDrawingBoard::OnDrawnRoomSlotHovered(UDrawnRoomSlot* DrawnRoomSlot)
{
	check(DrawComponent.IsValid());
	DrawComponent->OnRoomHovered.Broadcast(DrawnRoomSlot->GetInventoryItem().Get(), DrawnRoomSlot->GetGridIndex());
}

void UDrawingBoard::OnDrawnRoomSlotUnhovered(UDrawnRoomSlot* DrawnRoomSlot)
{
	check(DrawComponent.IsValid());
	DrawComponent->OnRoomUnhovered.Broadcast(DrawnRoomSlot->GetInventoryItem().Get(), DrawnRoomSlot->GetGridIndex());
}

void UDrawingBoard::OnRedrawButtonClicked()
{
	check(DrawComponent.IsValid());
	DrawComponent->Server_Redraw();
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

void UDrawingBoard::SetRedrawCount(int32 Count) const
{
	if (Count > 0)
	{
		HorizontalBox_RedrawBox->SetVisibility(ESlateVisibility::Visible);
		Text_RedrawCount->SetText(FText::AsNumber(Count));
	}
	else
	{
		HorizontalBox_RedrawBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDrawingBoard::SetDrawnRoomSlotImage(const UDrawnRoomSlot* DrawnRoomSlot, const FGridFragment* GridFragment, const FImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	DrawnRoomSlot->SetRoomIcon(Brush);
}

FVector2D UDrawingBoard::GetDrawSize(const FGridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	return  GridFragment->GetGridSize() * IconTileWidth;
}
