// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Display.generated.h"

class UKnowledgeComponent;

UCLASS()
class DRAWINVENTORY_API ADisplay : public AActor
{
	GENERATED_BODY()

public:
	ADisplay();

	void SetupDisplay(const FIntPoint& Coordinates);
	void SetupKnowledgeComponent(UKnowledgeComponent* KnowledgeComponent, int32 Integer) const;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> DisplayMesh;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TArray<UMaterialInterface*> Numerics;

	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UKnowledgeComponent> KnowledgeComponent_X;

	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UKnowledgeComponent> KnowledgeComponent_Y;
};
