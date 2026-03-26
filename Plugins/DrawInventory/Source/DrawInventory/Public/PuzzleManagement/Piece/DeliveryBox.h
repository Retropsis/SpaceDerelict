// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "RewardBox.h"
#include "DeliveryBox.generated.h"

UCLASS()
class DRAWINVENTORY_API ADeliveryBox : public ARewardBox
{
	GENERATED_BODY()

public:
	ADeliveryBox();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetupDeliveryPanel(const TMap<FIntPoint, UTexture2D*>& Panel);
};
