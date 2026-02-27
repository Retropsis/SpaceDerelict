// Retropsis 2026


#include "World/Utility/LevelUtility.h"

void ULevelUtility::GetAllActorsOfClassFromStreamLevel(ULevelStreaming* LevelStreaming, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors)
{
	if (!IsValid(LevelStreaming) || !IsValid(LevelStreaming->GetLoadedLevel()) || !IsValid(ActorClass)) return;
	
	// OutActors = LevelStreaming->GetLoadedLevel()->Actors.FilterByPredicate([ActorClass] (const TObjectPtr<AActor>& Actor)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Actor Name %s"), *Actor->GetName());
	// 	return Actor && Actor->GetClass() == ActorClass;
	// });
	
	for (AActor* Actor : LevelStreaming->GetLoadedLevel()->Actors)
	{
		if (Actor && Actor->IsA(ActorClass))
		{
			OutActors.Add(Actor);
		}
	}
}

AActor* ULevelUtility::GetFirstActorOfClassFromStreamLevel(ULevelStreaming* LevelStreaming, TSubclassOf<AActor> ActorClass)
{
	if (!IsValid(LevelStreaming) || !IsValid(LevelStreaming->GetLoadedLevel()) || !IsValid(ActorClass)) return nullptr;
	
	for (AActor* Actor : LevelStreaming->GetLoadedLevel()->Actors)
	{
		if (Actor && Actor->IsA(ActorClass))
		{
			return Actor;
		}
	}
	return nullptr;
}
