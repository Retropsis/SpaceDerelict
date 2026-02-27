// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelUtility.generated.h"

/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API ULevelUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (WorldContext = WorldContextObject))
	static void GetAllActorsOfClassFromStreamLevel(ULevelStreaming* LevelStreaming, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (WorldContext = WorldContextObject))
	static AActor* GetFirstActorOfClassFromStreamLevel(ULevelStreaming* LevelStreaming, TSubclassOf<AActor> ActorClass);
};
