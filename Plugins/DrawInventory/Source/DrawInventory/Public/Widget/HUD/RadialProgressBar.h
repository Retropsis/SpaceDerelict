// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadialProgressBar.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API URadialProgressBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	void ConstructDynamicMaterial();
	void SetPercentage(float Percentage);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartVisualEffect();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnCompletedVisualEffect();

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UMaterialInterface> RadialMaterial;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Radial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MID_Radial;
};
