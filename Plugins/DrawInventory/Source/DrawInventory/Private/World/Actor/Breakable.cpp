// Retropsis 2026


#include "World/Actor/Breakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ABreakable::ABreakable()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetupAttachment(GetRootComponent());
	
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	GeometryCollection->SetupAttachment(GetRootComponent());
	GeometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetSimulatePhysics(false);
	GeometryCollection->SetVisibility(false);
}

void ABreakable::GetHit_Implementation(const FVector& ImpactPoint)
{
	Mesh->DestroyComponent();
	GeometryCollection->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GeometryCollection->SetSimulatePhysics(true);
	GeometryCollection->SetVisibility(true);
	SpawnReward();
}

