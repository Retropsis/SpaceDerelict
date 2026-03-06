// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UActiveEquipmentWidget;
struct FGameplayTag;
class UHUDCounter;
class UHorizontalBox;
class UInfoMessage;


UCLASS()
class DRAWINVENTORY_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "DrawInventory")
	void ShowPickupMessage(const FString& Message);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "DrawInventory")
	void HidePickupMessage();

	void InitializeActiveEquipmentWidget();
	void InitializeHUDCounters();
	void ToggleHUD(bool bShow);

	UFUNCTION()
	void UpdateHUDCounter(const FGameplayTag& ItemType, int32 NewCount);

	UFUNCTION()
	void UpdateActiveEquipmentWidget(const FGameplayTag& EquipmentType, bool bNewlyEquipped);
	
	UFUNCTION()
	void OnActiveEquipped(const FGameplayTag& EquipmentType);
	
	UFUNCTION()
	void OnActiveUnequipped(const FGameplayTag& EquipmentType) {}

private:	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInfoMessage> InfoMessage;

	UPROPERTY()
	TMap<FGameplayTag, UHUDCounter*> HUDCounters;

	UPROPERTY()
	TArray<TObjectPtr<UActiveEquipmentWidget>> ActiveEquipmentWidgets;

	UFUNCTION()
	void OnNoRoom();
	
	UFUNCTION()
	void OnNoItemOfTypeAndAmount(const FName& ItemName);
};
