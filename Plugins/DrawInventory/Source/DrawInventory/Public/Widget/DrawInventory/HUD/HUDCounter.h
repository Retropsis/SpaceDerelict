// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "HUDCounter.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class DRAWINVENTORY_API UHUDCounter : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	void SetIcon(UTexture2D* Icon) const;
	void SetCount(int32 Count);
	FGameplayTag GetItemType() const {return ItemType; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnNewCountVisualEffects();

private:
	UPROPERTY(EditAnywhere, Category ="DrawInventory", meta=(Categories="Item.Currency"))
	FGameplayTag ItemType;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	float IconSize{64.f};
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	bool bAlwaysShow{true};

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Icon;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Count;
};
