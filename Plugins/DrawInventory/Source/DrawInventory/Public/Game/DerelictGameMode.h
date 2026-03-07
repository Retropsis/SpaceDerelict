// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DerelictGameMode.generated.h"

class UKnowledgeData;
/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API ADerelictGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UKnowledgeData* GetKnowledgeData() const { return KnowledgeData; }

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UKnowledgeData> KnowledgeData;
	
};
