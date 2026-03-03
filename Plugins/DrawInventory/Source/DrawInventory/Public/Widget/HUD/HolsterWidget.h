// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "HolsterWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API UHolsterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	void SetIcon(bool bEquipped) const;
	FGameplayTag GetEquipmentType() const {return EquipmentType; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnHolsterVisualEffects();

private:
	UPROPERTY(EditAnywhere, Category ="DrawInventory", meta=(Categories="Item"))
	FGameplayTag EquipmentType;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TObjectPtr<UTexture2D> Icon_Equipped;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TObjectPtr<UTexture2D> Icon_Unequipped;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Holster;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Key;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	float IconSize{64.f};
};
