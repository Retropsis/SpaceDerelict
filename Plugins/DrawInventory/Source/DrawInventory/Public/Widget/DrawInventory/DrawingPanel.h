// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DrawingPanel.generated.h"

class UDrawComponent;
struct FImageFragment;
struct FGridFragment;
struct FGameplayTag;
class UInventoryItem;
class UButton;
class UTextBlock;
class UHorizontalBox;
class UDrawnRoomSlot;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API UDrawingPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	void ClearDrawingBoard();
	void DrawRoom(UInventoryItem* Room, int32 Index, int32 Yaw, const FGameplayTag& Layer, bool bRequirementMet);
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
	
	void SetDrawnRoomSlotImage(const UDrawnRoomSlot* DrawnRoomSlot, const FGridFragment* GridFragment, const FImageFragment* ImageFragment) const;
	FVector2D GetDrawSize(const FGridFragment* GridFragment) const;
	
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
	
	UPROPERTY()
	TArray<TObjectPtr<UDrawnRoomSlot>> DrawnRoomSlots;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	float TileSize{240};
	
	TWeakObjectPtr<UDrawComponent> DrawComponent;
};
