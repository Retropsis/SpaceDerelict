// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "KnowledgeWidget.h"
#include "Blueprint/UserWidget.h"
#include "SymbolWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API USymbolWidget : public UKnowledgeWidget
{
	GENERATED_BODY()

public:
	void SetImage(UTexture2D* Texture);
	void SetText(FText Text);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Symbol;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Symbol;
};
