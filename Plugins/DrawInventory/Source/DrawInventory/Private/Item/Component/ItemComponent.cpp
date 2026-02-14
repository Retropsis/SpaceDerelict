// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Component/ItemComponent.h"
#include "Net/UnrealNetwork.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UItemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

void UItemComponent::InitializeItemManifest(FItemManifest CopyOfItemManifest)
{
	ItemManifest = CopyOfItemManifest;
}

void UItemComponent::PickedUp()
{
	OnPickedUp();
	GetOwner()->Destroy();
}

