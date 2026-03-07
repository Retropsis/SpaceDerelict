
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "KnowledgeTypes.generated.h"

USTRUCT()
struct FKnowledgeFragment
{
	GENERATED_BODY()

public:
	FKnowledgeFragment() {}
	FKnowledgeFragment(const FKnowledgeFragment&) = default;
	FKnowledgeFragment& operator=(const FKnowledgeFragment&) = default;
	FKnowledgeFragment(FKnowledgeFragment&&) = default;
	FKnowledgeFragment& operator=(FKnowledgeFragment&&) = default;
	virtual ~FKnowledgeFragment() {}
	FGameplayTag GetFragmentTag() const { return  FragmentTag; }

private:
	UPROPERTY(EditAnywhere, Category ="DrawInventory", meta=(Categories="Knowledge.Fragment"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FKnowledgeImage : public FKnowledgeFragment
{
	GENERATED_BODY()

public:
	UTexture2D* GetImage() const { return Image; }
	void SetImage(UTexture2D* NewImage) { Image = NewImage; }

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UTexture2D> Image;
};

USTRUCT(BlueprintType)
struct FKnowledgeText : public FKnowledgeFragment
{
	GENERATED_BODY()

public:
	FText GetText() const { return Text; }
	void SetText(const FText& NewText) { Text = NewText; }

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FText Text;
};