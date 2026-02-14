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

/**
 * 
 */
UCLASS()
class DRAWINVENTORY_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
	

protected:
	virtual void BeginPlay() override;

private:
	virtual void SetupInputComponent() override;
	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceForItem();
	
	void ToggleDrawingBoard();
	
	UPROPERTY(EditDefaultsOnly, Category=Inventory)
	TObjectPtr<UInputMappingContext> DefaultIMC;
	
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
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
	TWeakObjectPtr<UDrawComponent> DrawComponent;
	TWeakObjectPtr<UDoorComponent> DoorComponent;
};
