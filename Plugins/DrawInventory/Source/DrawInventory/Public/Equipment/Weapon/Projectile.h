// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(Abstract)
class DRAWINVENTORY_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="DrawInventory", meta = (DisplayName = "On Projectile Hit"))
	void OnProjectileHit(const FHitResult& Hit);
	
	UFUNCTION(BlueprintImplementableEvent, Category="DrawInventory")
	void ConstructFieldSystem(const FVector& FieldLocation);
	
private:
	void ExplosionCheck(const FVector& ExplosionCenter);
	void ProcessHit(AActor* HitActor, UPrimitiveComponent* HitComp, const FVector& HitLocation, const FVector& HitDirection);
	void OnDeferredDestruction();
	
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	bool bDamageOwner = false;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta = (ClampMin = 0, ClampMax = 50000))
	float PhysicsForce = 100.0f;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta = (ClampMin = 0, ClampMax = 100))
	float HitDamage = 25.0f;

	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TSubclassOf<UDamageType> HitDamageType;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	bool bExplodeOnHit = false;

	UPROPERTY(EditAnywhere, Category="DrawInventory", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float DeferredDestructionTime = 1.0f;

	FTimerHandle DestructionTimer;
	bool bHit = false;
};