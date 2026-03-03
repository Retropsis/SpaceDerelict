// Retropsis 2026

#include "Widget/DrawInventory/DrawingPanel.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "DrawManagement/Component/DrawComponent.h"
#include "DrawManagement/Utility/DrawingUtility.h"
#include "Item/InventoryItem.h"
#include "Item/Fragment/ItemFragment.h"
#include "Widget/DrawInventory/SlottedRoom/DrawnRoomSlot.h"

void UDrawingPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	DrawComponent = UDrawingUtility::GetDrawComponent(GetOwningPlayer());
	Button_Redraw->OnClicked.AddDynamic(this, &ThisClass::OnRedrawButtonClicked);
}

void UDrawingPanel::ClearDrawingBoard()
{
	HorizontalBox_DrawnRoomSlots->ClearChildren();
	DrawnRoomSlots.Empty();
}

void UDrawingPanel::DrawRoom(UInventoryItem* Room, int32 Index, int32 Yaw, const FGameplayTag& Layer, bool bRequirementMet)
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
	if (const FPuzzleFragment* PuzzleFragment = Room->GetItemManifest().GetFragmentOfType<FPuzzleFragment>())
	{
		DrawnRoomSlot->SetPuzzleIcon(PuzzleFragment->GetPuzzleIcon());
	}
	DrawnRoomSlot->OnDrawVisualEffects();
	DrawnRoomSlots.Add(DrawnRoomSlot);
}

void UDrawingPanel::SetRedrawCount(int32 Count) const
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

void UDrawingPanel::OnDrawnRoomSlotClicked(UDrawnRoomSlot* DrawnRoomSlot)
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

void UDrawingPanel::OnDrawnRoomSlotHovered(UDrawnRoomSlot* DrawnRoomSlot)
{
	check(DrawComponent.IsValid());
	DrawComponent->OnRoomHovered.Broadcast(DrawnRoomSlot->GetInventoryItem().Get(), DrawnRoomSlot->GetGridIndex());
}

void UDrawingPanel::OnDrawnRoomSlotUnhovered(UDrawnRoomSlot* DrawnRoomSlot)
{
	check(DrawComponent.IsValid());
	DrawComponent->OnRoomUnhovered.Broadcast(DrawnRoomSlot->GetInventoryItem().Get(), DrawnRoomSlot->GetGridIndex());
}

void UDrawingPanel::OnRedrawButtonClicked()
{
	check(DrawComponent.IsValid());
	DrawComponent->Server_Redraw();
}

void UDrawingPanel::SetDrawnRoomSlotImage(const UDrawnRoomSlot* DrawnRoomSlot, const FGridFragment* GridFragment, const FImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	DrawnRoomSlot->SetRoomIcon(Brush);
}

FVector2D UDrawingPanel::GetDrawSize(const FGridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	return  GridFragment->GetGridSize() * IconTileWidth;
}
