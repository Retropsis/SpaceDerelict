// Retropsis 2026

#include "PuzzleManagement/PuzzleComponent.h"
#include "DrawManagement/Room/ItemSpawner.h"
#include "Interaction/InteractionComponent.h"
#include "Item/ItemTags.h"

void UPuzzleComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	// if (PropertyChangedEvent.Property->GetName() == TEXT("ItemSpawnerClass"))
	// {
		// ConstructSpawners();
	// }
}

void UPuzzleComponent::ConstructSpawners()
{
	// if (IsValid(ItemSpawnerClass))
	// {
	// 	UItemSpawner* ItemSpawner_0 = GetOwner()->CreateDefaultSubobject<UItemSpawner>(TEXT("ItemSpawner_BoxKey"), ItemSpawnerClass, UItemSpawner::StaticClass());
	// 	// ItemSpawner_0->RegisterComponent();
	// 	// GetOwner()->AddInstanceComponent(ItemSpawner_0);
	// 	ItemSpawner_0->SetSpawnerTag(Item::Currency::BoxKey);
	// 	ItemSpawners.Add(ItemSpawner_0);
	//
	// 	UItemSpawner* ItemSpawner_1 = GetOwner()->CreateDefaultSubobject<UItemSpawner>(TEXT("ItemSpawner_1"), ItemSpawnerClass);
	// 	// ItemSpawner_1->RegisterComponent();
	// 	// GetOwner()->AddInstanceComponent(ItemSpawner_1);
	// 	ItemSpawners.Add(ItemSpawner_1);
	//
	// 	UItemSpawner* ItemSpawner_2 = GetOwner()->CreateDefaultSubobject<UItemSpawner>(TEXT("ItemSpawner_2"), ItemSpawnerClass);
	// 	// ItemSpawner_2->RegisterComponent();
	// 	// GetOwner()->AddInstanceComponent(ItemSpawner_2);
	// 	ItemSpawners.Add(ItemSpawner_2);
	//
	// 	UItemSpawner* ItemSpawner_3 = GetOwner()->CreateDefaultSubobject<UItemSpawner>(TEXT("ItemSpawner_3"), ItemSpawnerClass);
	// 	// ItemSpawner_3->RegisterComponent();
	// 	// GetOwner()->AddInstanceComponent(ItemSpawner_3);
	// 	ItemSpawners.Add(ItemSpawner_3);
	// }
}

void UPuzzleComponent::ConstructPuzzle()
{
	// for (auto ErrorHist : BoxClasses)
	// {
	// 	const FString NewLabel = FString::Printf(TEXT("AC_ItemSpawner"));
	// 	UItemSpawner* ItemSpawner = NewObject<UItemSpawner>(GetOwner(), ItemSpawnerClass, FName(NewLabel));
	// 	if (!IsValid(ItemSpawner)) return;
	// 	ItemSpawner->RegisterComponent();
	// 	GetOwner()->AddInstanceComponent(ItemSpawner);
	// }
}

