// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KnowledgeLog.generated.h"

class USymbolWidget;
class UKnowledgePage;
struct FGameplayTag;
class UKnowledgeComponent;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API UKnowledgeLog : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnKnowledgeReceived(const UKnowledgeComponent* KnowledgeComponent) const;	

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UKnowledgePage> Page_Symbols;

	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<USymbolWidget> SymbolWidgetClass;
};
