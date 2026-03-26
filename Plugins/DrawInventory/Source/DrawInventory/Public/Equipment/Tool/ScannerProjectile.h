// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Weapon/Projectile.h"
#include "ScannerProjectile.generated.h"

UCLASS()
class DRAWINVENTORY_API AScannerProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AScannerProjectile();
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void ProcessHit(AActor* HitActor, UPrimitiveComponent* HitComp, const FVector& HitLocation, const FVector& HitDirection) override;
};
