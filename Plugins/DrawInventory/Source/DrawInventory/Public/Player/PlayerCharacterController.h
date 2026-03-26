// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

struct FInputActionValue;
class APlayerCharacter;
class UDrawComponent;
class UDoorComponent;
class UInventoryComponent;
class UHUDWidget;
class UInputAction;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerPositionUpdated, const FVector2D&, Location, const float, Angle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerLayerUpdated,  const FGameplayTag&, Layer);

UCLASS()
class DRAWINVENTORY_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();
	virtual void Tick(float DeltaTime) override;
	void SetTimerPlayerPositionUpdate();
	float GetAOPitch() const { return AO_Pitch; }
	bool IsGloveRaised() const { return bGloveRaised; }
	bool IsAimDownSight() const { return bAimDownSight; }
	virtual void RestartLevel() override;
	UHUDWidget* GetHUDWidget() const { return HUDWidget; }
	
	void SetDefaultMappingToAzerty() const;
	void SetDefaultMappingToQwerty() const;

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
	
	UFUNCTION()
	void ToggleHUD(bool bShow);

	void SetSavedPlayerLocation(const FVector& Location);
	FVector GetSavedPlayerLocation() const { return SavedPlayerLocation; }
	FRotator GetSavedPlayerRotation() const { return SavedPlayerRotation; }

	FPlayerPositionUpdated OnPlayerPositionUpdated;

	UPROPERTY(BlueprintAssignable)
	FPlayerLayerUpdated OnPlayerLayerUpdated;

protected:
	virtual void BeginPlay() override;
	APlayerCharacter* GetPlayerCharacter();
	virtual void OnPossess(APawn* aPawn) override;

private:
	virtual void SetupInputComponent() override;
	void PrimaryInteract();
	void SecondaryInteract(bool bADS);
	void CreateHUDWidget();
	void TraceForItem();
	void CalculateAOPitch();
	void ToggleGlove(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartFiring();
	void StopFiring();
	void ToggleKnowledgeLog();
	void ToggleMainMenu();
	void ToggleWeapon(int32 Index);

	UFUNCTION(BlueprintCallable, Category="DrawInventory")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category="DrawInventory")
	virtual void DoJumpEnd();
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TObjectPtr<UInputMappingContext> QwertyIMC;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TObjectPtr<UInputMappingContext> AzertyIMC;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TObjectPtr<UInputAction> SecondaryInteractAction;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TObjectPtr<UInputAction>EquipmentAction_1;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TObjectPtr<UInputAction>EquipmentAction_2;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TObjectPtr<UInputAction> ToggleInventoryAction;
	
	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TObjectPtr<UInputAction> ToggleGloveAction;

	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TObjectPtr<UInputAction> MouseLookAction;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TObjectPtr<UInputAction> KnowledgeAction;
	
	UPROPERTY(EditAnywhere, Category ="DrawInventory")
	TObjectPtr<UInputAction> MenuAction;

	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UHUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	double TraceLength;

	UPROPERTY(EditDefaultsOnly, Category="DrawInventory")
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel;

	TWeakObjectPtr<APlayerCharacter> PlayerCharacter;
	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;
	TWeakObjectPtr<UActorComponent> ThisComponent;
	TWeakObjectPtr<UActorComponent> LastComponent;
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
	TWeakObjectPtr<UDrawComponent> DrawComponent;
	float AO_Pitch;
	bool bGloveRaised;
	FVector SavedPlayerLocation = FVector::ZeroVector;
	FRotator SavedPlayerRotation = FRotator::ZeroRotator;
	FGameplayTag PreviousLayer = FGameplayTag::EmptyTag;
	bool bAimDownSight{false};
};
