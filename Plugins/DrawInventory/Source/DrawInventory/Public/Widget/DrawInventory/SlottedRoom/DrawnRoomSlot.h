// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DrawnRoomSlot.generated.h"

class USizeBox;
class UImage;
class UTextBlock;
class UInventoryItem;
class UHorizontalBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDrawnRoomSloStatusChanged, UDrawnRoomSlot*, GridSlot);

UCLASS()
class DRAWINVENTORY_API UDrawnRoomSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	void SetRoomIcon(const FSlateBrush& Brush) const;
	void SetRoomName(const FText& Name) const;
	void SetRequirement(UTexture2D* Icon, int32 Amount, bool bRequirementMet) const;
	void SetValuable(const TMap<UTexture2D* , int32>& Valuables) const;
	TWeakObjectPtr<UInventoryItem> GetInventoryItem() const { return InventoryItem; }
	void SetInventoryItem(UInventoryItem* Item);
	void SetSizeBox(float TileSize) const;
	void SetGridIndex(int32 Index) { GridIndex = Index; }
	int32 GetGridIndex() const { return GridIndex; }
	void SetYaw(int32 NewYaw) { Yaw = NewYaw; }
	int32 GetYaw() const { return Yaw; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnSlotHoveredVisualEffects();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSlotUnhoveredVisualEffects();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDrawVisualEffects();
	
	// UEquippedSlottedItem* OnItemEquipped(UInventoryItem* Item, const FGameplayTag& Type, float TileSize);

	FDrawnRoomSloStatusChanged OnDrawnRoomSlotClicked;
	FDrawnRoomSloStatusChanged OnDrawnRoomSlotHovered;
	FDrawnRoomSloStatusChanged OnDrawnRoomSlotUnhovered;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_RoomName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_DrawRoomSlot;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Valuable;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Requirement;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Requirement;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	FLinearColor RequirementMetColor;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	FLinearColor RequirementNotMatchedColor;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Item.Room"))
	FGameplayTag RoomType;
	
	TWeakObjectPtr<UInventoryItem> InventoryItem;
	int32 GridIndex{ INDEX_NONE };
	int32 Yaw{ INDEX_NONE };
};
