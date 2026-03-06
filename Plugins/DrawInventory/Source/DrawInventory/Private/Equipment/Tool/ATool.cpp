// Retropsis 2026


#include "Equipment/Tool/ATool.h"


// Sets default values
AATool::AATool()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AATool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AATool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

