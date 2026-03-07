// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "KnowledgePage.generated.h"

class UKnowledgeWidget;
class UVerticalBox;


UCLASS()
class DRAWINVENTORY_API UKnowledgePage : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddWidget(UKnowledgeWidget* NewWidget);
	FGameplayTag GetKnowledgeCategory() const { return KnowledgeCategory; }

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Log;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FGameplayTag KnowledgeCategory = FGameplayTag::EmptyTag;

	TArray<TObjectPtr<UKnowledgeWidget>> KnowledgeWidgets;
};
