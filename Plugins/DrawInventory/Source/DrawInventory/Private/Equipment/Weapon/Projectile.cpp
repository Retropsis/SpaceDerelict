// Retropsis 2026

#include "Equipment/Weapon/Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/HitInterface.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	CollisionComponent->SetSphereRadius(16.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bShouldBounce = true;

	// set the default damage type
	// HitDamageType = UDamageType::StaticClass();
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void AProjectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bHit)
	{
		return;
	}
	bHit = true;

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// MakeNoise(NoiseLoudness, GetInstigator(), GetActorLocation(), NoiseRange, NoiseTag);

	if (bExplodeOnHit)
	{
		ExplosionCheck(GetActorLocation());
	}
	else
	{
		ProcessHit(Other, OtherComp, Hit.ImpactPoint, -Hit.ImpactNormal);
	}

	if (DeferredDestructionTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(DestructionTimer, this, &AProjectile::OnDeferredDestruction, DeferredDestructionTime, false);
	}
	else
	{
		Destroy();
	}

	OnProjectileHit(Hit);
	ConstructFieldSystem(GetActorLocation());
}

void AProjectile::ProcessHit(AActor* HitActor, UPrimitiveComponent* HitComp, const FVector& HitLocation, const FVector& HitDirection)
{
	if (ACharacter* HitCharacter = Cast<ACharacter>(HitActor))
	{
		if (HitCharacter != GetOwner() || bDamageOwner)
		{
			UGameplayStatics::ApplyDamage(HitCharacter, HitDamage, GetInstigator()->GetController(), this, HitDamageType);
		}
	}

	if (IHitInterface* HitInterface = Cast<IHitInterface>(HitActor))
	{
		HitInterface->Execute_GetHit(HitActor, HitLocation);
	}

	if (HitComp->IsSimulatingPhysics())
	{
		HitComp->AddImpulseAtLocation(HitDirection * PhysicsForce, HitLocation);
	}
}

void AProjectile::ExplosionCheck(const FVector& ExplosionCenter)
{

}

void AProjectile::OnDeferredDestruction()
{
	Destroy();
}
