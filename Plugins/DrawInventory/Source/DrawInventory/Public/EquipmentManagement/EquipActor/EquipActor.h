// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "EquipActor.generated.h"

UCLASS()
class DRAWINVENTORY_API AEquipActor : public AActor
{
	GENERATED_BODY()

public:
	AEquipActor();
	FGameplayTag GetEquipmentType() const { return EquipmentType; }
	void SetEquipmentType(FGameplayTag Type) { EquipmentType = Type; }

	virtual void InitializeEquipment() {}
	virtual void OnEquip(APawn* Pawn) {}

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FGameplayTag EquipmentType;
};
