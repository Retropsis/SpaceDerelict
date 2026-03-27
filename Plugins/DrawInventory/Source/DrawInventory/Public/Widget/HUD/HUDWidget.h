// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UHelpPage;
class UMainMenu;
class UKnowledgeComponent;
class UKnowledgeLog;
class URadialProgressBar;
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
	void InitializeMenus();
	void CloseKnowledgeLog();
	void OpenKnowledgeLog();
	void ToggleKnowledgeLog();
	void SetInputModeGameOnly() const;
	void SetInputModeGameAndUI() const;
	void OpenMainMenu();
	void ToggleMainMenu();
	void ToggleHelpPage();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "DrawInventory")
	void ShowPickupMessage(const FString& Message);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "DrawInventory")
	void HidePickupMessage();

	void InitializeActiveEquipmentWidget();
	void InitializeHUDCounters();
	void ToggleHUD(bool bShow);
	
	UFUNCTION()
	void CloseMainMenu();

	UFUNCTION()
	void UpdateHUDCounter(const FGameplayTag& ItemType, int32 NewCount);

	UFUNCTION()
	void UpdateActiveEquipmentWidget(const FGameplayTag& EquipmentType, bool bNewlyEquipped);
	
	UFUNCTION()
	void UpdateActiveEquipmentADS(const FGameplayTag& EquipmentType, bool bAimDownSight);
	
	UFUNCTION()
	void OnActiveEquipped(const FGameplayTag& EquipmentType);
	
	UFUNCTION()
	void OnActiveUnequipped(const FGameplayTag& EquipmentType) {}
	
	UFUNCTION()
	void OnScanStarted();

	UFUNCTION()
	void OnScanProgress(float Percentage);
	
	UFUNCTION()
	void OnScanComplete(const UKnowledgeComponent* KnowledgeComponent);

private:	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInfoMessage> InfoMessage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URadialProgressBar> RadialProgressBar;

	UPROPERTY()
	TMap<FGameplayTag, UHUDCounter*> HUDCounters;

	UPROPERTY()
	TArray<TObjectPtr<UActiveEquipmentWidget>> ActiveEquipmentWidgets;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UKnowledgeLog> KnowledgeLog;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMainMenu> MainMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHelpPage> HelpPage;

	bool bKnowledgeLogOpen{false};
	bool bMainMenuOpen{false};
	bool bHelpPageOpen{false};

	UFUNCTION()
	void OnNoRoom();
	
	UFUNCTION()
	void OnNoItemOfTypeAndAmount(const FName& ItemName);
};
