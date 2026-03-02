// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Interaction/HitInterface.h"
#include "PuzzleManagement/Piece/RewardBox.h"
#include "Breakable.generated.h"

UCLASS()
class DRAWINVENTORY_API ABreakable : public ARewardBox, public IHitInterface
{
	GENERATED_BODY()

public:
	ABreakable();

private:
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;
};
