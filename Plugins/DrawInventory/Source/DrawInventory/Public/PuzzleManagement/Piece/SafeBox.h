// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SafeBox.generated.h"

class UWidgetComponent;
class UDigitWidget;
class UDigitComponent;
class UBoxComponent;


UCLASS()
class DRAWINVENTORY_API ASafeBox : public AActor
{
	GENERATED_BODY()

public:
	ASafeBox();
	void SetSafeBoxCode(const FString& Code);
	void SpawnReward() const;
	void SetLootItemClass(const TSubclassOf<AActor>& Class) { LootItemClass = Class; }

	UFUNCTION(BlueprintImplementableEvent)
	void Unlock();

	UFUNCTION()
	void OnNumericInput(const FName& Digit);

protected:
	virtual void BeginPlay() override;

private:
	void ResetInputCode();
	UWidgetComponent* ConstructDigitWidgetComponent(const FName& Name);
	
	TArray<int32> SafeBoxCode;
	TArray<int32> InputCode;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<USkeletalMeshComponent> SafeBoxMesh;
	
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TArray<TObjectPtr<UDigitComponent>> Digits;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<USceneComponent> NumericPadRoot;
	
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> DigitComponentClass;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> Digit_1;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> Digit_2;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> Digit_3;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> Digit_4;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> Digit_5;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> Digit_6;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> Digit_7;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> Digit_8;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> Digit_9;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> Digit_0;
 
	UPROPERTY(VisibleAnywhere, Category="DrawInventory")
	TObjectPtr<UDigitComponent> Digit_Enter;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TSubclassOf<UDigitWidget> DigitWidgetClass;

	UPROPERTY()
	TObjectPtr<UDigitWidget> DigitWidget;
	
	UPROPERTY(EditAnywhere, Category="DrawInventory")
	TObjectPtr<UWidgetComponent> DigitWidgetComponent;

	bool bUnlocked = false;
	TSubclassOf<AActor> LootItemClass;
};
