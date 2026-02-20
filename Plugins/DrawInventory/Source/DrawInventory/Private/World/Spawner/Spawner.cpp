// Retropsis 2026

#include "World/Spawner/Spawner.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SpawnTransform = CreateDefaultSubobject<USceneComponent>("SpawnTransform");
	SpawnTransform->SetupAttachment(Root);

	BoxVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("BoxVisualizer");
	BoxVisualizer->SetupAttachment(SpawnTransform);
	BoxVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxVisualizer->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxVisualizer->SetGenerateOverlapEvents(false);
}

FTransform ASpawner::GetSpawnTransform() const
{
	return SpawnTransform->GetComponentTransform();
}

