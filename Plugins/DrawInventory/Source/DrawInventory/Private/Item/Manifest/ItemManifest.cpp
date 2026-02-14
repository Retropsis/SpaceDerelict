
#include "Item/Manifest/ItemManifest.h"
#include "Item/InventoryItem.h"
#include "Item/Component/ItemComponent.h"
#include "Item/Fragment/ItemFragment.h"
#include "Widget/Composite/CompositeBase.h"

UInventoryItem* FItemManifest::Manifest(UObject* NewOuter)
{
	UInventoryItem* Item = NewObject<UInventoryItem>(NewOuter, UInventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	for (auto& Fragment : Item->GetItemManifestMutable().GetFragmentsMutable())
	{
		Fragment.GetMutable().Manifest();
	}
	ClearFragments();

	return Item;
}

void FItemManifest::ClearFragments()
{
	for (auto& Fragment : Fragments)
	{
		Fragment.Reset();
	}
	Fragments.Empty();
}

void FItemManifest::SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!IsValid(PickupActorClass) || !IsValid(WorldContextObject)) return;

	AActor* SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickupActorClass, SpawnLocation, SpawnRotation);
	if (!IsValid(SpawnedActor)) return;

	// Setup ItemComponent, Category, Type, etc
	UItemComponent* ItemComponent = SpawnedActor->FindComponentByClass<UItemComponent>();
	check(ItemComponent);

	ItemComponent->InitializeItemManifest(*this);
}

void FItemManifest::AssimilateInventoryFragments(UCompositeBase* Composite) const
{
	const auto& InventoryItemFragments = GetAllFragmentsOfType<FInventoryItemFragment>();
	for (const auto* Fragment : InventoryItemFragments)
	{
		Composite->ApplyFunction([Fragment] (UCompositeBase* Widget)
		{
			Fragment->Assimilate(Widget);
		});
	}
}
