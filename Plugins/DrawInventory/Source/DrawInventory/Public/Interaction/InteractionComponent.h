// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();
	FString GetInteractionMessage() const { return InteractionMessage; }
	void SetInteractionMessage(FString NewMessage) { InteractionMessage = NewMessage; }

protected:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FString InteractionMessage;
};
