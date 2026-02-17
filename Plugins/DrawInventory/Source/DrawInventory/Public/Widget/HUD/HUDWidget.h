// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

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

	void InitializeHUDCounters();
	void ToggleHUD(bool bShow);

	UFUNCTION()
	void UpdateHUDCounter(const FGameplayTag& ItemType, int32 NewCount);

private:	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInfoMessage> InfoMessage;

	UPROPERTY()
	TMap<FGameplayTag, UHUDCounter*> HUDCounters;

	UFUNCTION()
	void OnNoRoom();
	
	UFUNCTION()
	void OnNoItemOfTypeAndAmount(const FName& ItemName);
};
