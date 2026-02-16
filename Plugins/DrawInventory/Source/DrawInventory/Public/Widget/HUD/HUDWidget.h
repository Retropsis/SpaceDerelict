// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UInfoMessage;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void ShowPickupMessage(const FString& Message);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void HidePickupMessage();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInfoMessage> InfoMessage;

	UFUNCTION()
	void OnNoRoom();
	
	UFUNCTION()
	void OnNoItemOfTypeAndAmount(const FName& ItemName);
};
