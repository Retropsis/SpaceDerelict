// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "EquipmentManagement/EquipActor/EquipActor.h"
#include "ATool.generated.h"

UCLASS()
class DRAWINVENTORY_API AATool : public AEquipActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AATool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
