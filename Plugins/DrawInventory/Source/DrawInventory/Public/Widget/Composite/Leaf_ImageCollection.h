// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Leaf.h"
#include "Leaf_ImageCollection.generated.h"

class UHorizontalBox;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API ULeaf_ImageCollection : public ULeaf
{
	GENERATED_BODY()

public:
	void SetCollectionVisibility(bool bCollapse) const;
	void AddImageToCollection(UTexture2D* Icon) const;
	void ClearImageCollection() const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Collection;
};
