// Retropsis 2026

#include "World/Level/Door/Door.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
}

