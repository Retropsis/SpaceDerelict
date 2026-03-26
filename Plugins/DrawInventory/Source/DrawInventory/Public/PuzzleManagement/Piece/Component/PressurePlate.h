// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PressurePlate.generated.h"

class UBoxComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPressurePlateActivated, const FGameplayTag&, Symbol);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DRAWINVENTORY_API APressurePlate : public AActor
{
	GENERATED_BODY()

public:
	APressurePlate();
	FGameplayTag GetSymbol() const {	return Symbol; }
	
	UFUNCTION()
	void OnPressureBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPressureEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnPressurePlateActivation();
	
	UFUNCTION()
	void OnSymbolLocked(const FGameplayTag& InSymbol);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSymbolLockedVisualEffects();

	UFUNCTION(BlueprintImplementableEvent)
	void PressurePlateActivationEffects();
	
	FOnPressurePlateActivated OnPressurePlateActivated;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	float PressurePlateTime{1.25f};

	UPROPERTY(EditAnywhere, Category="DrawInventory", meta=(Categories="Knowledge.Symbol"))
	FGameplayTag Symbol = FGameplayTag::EmptyTag;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxOverlap;
	
	FTimerHandle PressurePlateTimer;
};
