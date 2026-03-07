// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Equipment/ActiveEquipActor.h"
#include "KnowledgeScanner.generated.h"

class UKnowledgeComponent;
class UCableComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScanProgress, float, Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScanComplete, const UKnowledgeComponent*, KnowledgeComponent);

UCLASS()
class DRAWINVENTORY_API AKnowledgeScanner : public AActiveEquipActor
{
	GENERATED_BODY()

public:
	AKnowledgeScanner();
	virtual void InitializeEquipment() override;
	virtual void StartActive() override;
	virtual void StopActive() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartScanProgress(float Duration);

	UFUNCTION(BlueprintCallable)
	bool IsScanning() const { return bIsScanning; };

	UFUNCTION(BlueprintCallable)
	void ScanComplete();

	UPROPERTY(BlueprintCallable)
	FOnScanProgress OnScanProgress;
	
	UPROPERTY(BlueprintCallable)
	FOnScanComplete OnScanComplete;
	
protected:
	virtual void BeginPlay() override;

private:
	virtual void Scan();
	void ScanLinkFailed();
	virtual void FireScanLink(const FVector& TargetLocation);
	FTransform CalculateTargetTransform(const FVector& TargetLocation) const;
	void OnDeferredDetachment();
	void OnDeferredUnlink();

	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	UCableComponent* LinkComponent;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float ScanRate = 0.5f;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float TimeToComplete = 2.f;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, Category="DrawInventory", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float MuzzleOffset = 10.0f;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory", meta = (ClampMin = 0, ClampMax = 90, Units = "Degrees"))
	float AimVariance = 0.0f;
	
	UPROPERTY(EditAnywhere, Category="Projectile|Destruction", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float DeferredDetachmentTime = 1.0f;
	
	UPROPERTY(EditAnywhere, Category="Projectile|Destruction", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float DeferredUnlinkTime = .2f;
	
	bool bIsScanning = false;
	float TimeOfLastScan = 0.0f;
	FTimerHandle RescanTimer;
	FTimerHandle ScanTimer;
	FTimerHandle DeferredDetachmentTimer;
	FTimerHandle DeferredUnlinkTimer;
	TWeakObjectPtr<UKnowledgeComponent> KnowledgeComponent;
};
