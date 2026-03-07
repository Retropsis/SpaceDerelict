// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "KnowledgeWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API UKnowledgeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetKnowledgeTag(const FGameplayTag& Tag) { KnowledgeTag = Tag; }
	FGameplayTag GetKnowledgeTag() const { return KnowledgeTag; }
	
private:
	FGameplayTag KnowledgeTag = FGameplayTag::EmptyTag;
};
