// Retropsis 2026

#include "PuzzleManagement/Piece/Component/PressurePlate.h"
#include "Components/BoxComponent.h"

APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BoxOverlap = CreateDefaultSubobject<UBoxComponent>(FName("BoxOverlap"));
	BoxOverlap->SetupAttachment(GetRootComponent());
}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	BoxOverlap->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPressureBeginOverlap);
	BoxOverlap->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnPressureEndOverlap);
}

void APressurePlate::OnPressureBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorld()->GetTimerManager().SetTimer(PressurePlateTimer, this, &ThisClass::OnPressurePlateActivation, PressurePlateTime, false);
}

void APressurePlate::OnPressureEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(PressurePlateTimer);
	PressurePlateTimer.Invalidate();
}

void APressurePlate::OnPressurePlateActivation()
{
	OnPressurePlateActivated.Broadcast(Symbol);
	PressurePlateActivationEffects();
	BoxOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

