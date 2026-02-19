// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionComponent.h"
#include "Item/Manifest/ItemManifest.h"
#include "ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UItemComponent : public UInteractionComponent
{
	GENERATED_BODY()

public:
	UItemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void InitializeItemManifest(FItemManifest CopyOfItemManifest);
	FItemManifest GetItemManifest() const { return ItemManifest; }
	FItemManifest& GetItemManifestMutable() { return ItemManifest; }
	void PickedUp();

	void ToggleItemMeshCollision(bool bEnabled) const;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void OnPickedUp();
	
private:
	UPROPERTY(Replicated, EditAnywhere, Category="Inventory")
	FItemManifest ItemManifest;
};
