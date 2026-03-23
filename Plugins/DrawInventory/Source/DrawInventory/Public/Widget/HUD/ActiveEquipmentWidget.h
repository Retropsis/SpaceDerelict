// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "ActiveEquipmentWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API UActiveEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	void SetIcon(bool bActivated) const;
	FGameplayTag GetEquipmentType() const {return EquipmentType; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnEquipVisualEffects();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimDownSightVisualEffects(bool bAimDownSight);

private:
	UPROPERTY(EditAnywhere, Category ="DrawInventory", meta=(Categories="Item"))
	FGameplayTag EquipmentType;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	FSlateBrush Brush_Activated;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	FSlateBrush Brush_Deactivated;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Equipment;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Key;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	float IconSize{64.f};
};
