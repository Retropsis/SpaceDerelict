// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interaction/InteractionComponent.h"
#include "DoorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UDoorComponent : public UInteractionComponent
{
	GENERATED_BODY()

public:
	UDoorComponent();
};
