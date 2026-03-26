// Retropsis 2026

#include "Equipment/Tool/ScannerProjectile.h"
#include "Components/SphereComponent.h"

AScannerProjectile::AScannerProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AScannerProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bHit)
	{
		return;
	}
	bHit = true;

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// MakeNoise(NoiseLoudness, GetInstigator(), GetActorLocation(), NoiseRange, NoiseTag);
	
	ProcessHit(Other, OtherComp, Hit.ImpactPoint, -Hit.ImpactNormal);

	if (DeferredDestructionTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(DestructionTimer, this, &AScannerProjectile::OnDeferredDestruction, DeferredDestructionTime, false);
	}
	else
	{
		Destroy();
	}

	OnProjectileHit(Hit);
}

void AScannerProjectile::ProcessHit(AActor* HitActor, UPrimitiveComponent* HitComp, const FVector& HitLocation, const FVector& HitDirection)
{
	// if (IHitInterface* HitInterface = Cast<IHitInterface>(HitActor))
	// {
	// 	HitInterface->Execute_GetHit(HitActor, HitLocation);
	// }
	
	// CollisionComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// CollisionComponent->AddImpulseAtLocation(HitDirection * PhysicsForce, HitLocation);
}

