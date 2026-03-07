// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "KnowledgeData.generated.h"

USTRUCT(BlueprintType)
struct FSymbolData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Texture = nullptr;
	
	UPROPERTY(EditAnywhere)
	FText Text = FText();

	UPROPERTY(EditAnywhere, meta=(Categories="Knowledge.Symbol"))
	FGameplayTag KnowledgeTag = FGameplayTag::EmptyTag;
};

UCLASS()
class DRAWINVENTORY_API UKnowledgeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FSymbolData GetSymbolDataByTag(const FGameplayTag& Tag);
	FSymbolData GetNumericSymbolDataByInt(int32 Integer);

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FSymbolData> Symbols;
};
