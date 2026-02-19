// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class UDrawComponent;
class UDoorComponent;
class UInventoryComponent;
class UHUDWidget;
class UInputAction;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerPositionUpdated, const FVector2D&, Location, const float, Angle);

UCLASS()
class DRAWINVENTORY_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();
	virtual void Tick(float DeltaTime) override;
	void SetTimerPlayerPositionUpdate() const;
	float GetAOPitch() const { return AO_Pitch; }

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
	
	UFUNCTION()
	void ToggleHUD(bool bShow);

	FPlayerPositionUpdated OnPlayerPositionUpdated;

protected:
	virtual void BeginPlay() override;

private:
	virtual void SetupInputComponent() override;
	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceForItem();
	void CalculateAOPitch();
	
	UPROPERTY(EditDefaultsOnly, Category=Inventory)
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;
	
	UPROPERTY(EditDefaultsOnly, Category=Inventory)
	TObjectPtr<UInputAction> PrimaryInteractAction;
	
	UPROPERTY(EditDefaultsOnly, Category=Inventory)
	TObjectPtr<UInputAction> ToggleInventoryAction;

	UPROPERTY(EditDefaultsOnly, Category=Inventory)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UHUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category=Inventory)
	double TraceLength;

	UPROPERTY(EditDefaultsOnly, Category=Inventory)
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel;

	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;
	TWeakObjectPtr<UActorComponent> ThisComponent;
	TWeakObjectPtr<UActorComponent> LastComponent;
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
	TWeakObjectPtr<UDrawComponent> DrawComponent;
	float AO_Pitch;
};
