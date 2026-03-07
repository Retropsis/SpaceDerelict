// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacterController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawManagement/Component/DrawComponent.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "Interaction/InteractionComponent.h"
#include "Item/ItemTags.h"
#include "Item/Fragment/FragmentTags.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "PuzzleManagement/Component/LockComponent.h"
#include "PuzzleManagement/Piece/Component/DigitComponent.h"
#include "Widget/HUD/HUDWidget.h"
#include "World/Level/Door/DoorComponent.h"

APlayerCharacterController::APlayerCharacterController()
{
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.0;
	InteractionTraceChannel = ECC_GameTraceChannel2;
	AO_Pitch = 0.f;
	bGloveRaised = false;
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceForItem();
	CalculateAOPitch();
}

void APlayerCharacterController::RestartLevel()
{
	Super::RestartLevel();
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	for (UInputMappingContext* CurrentContext : DefaultIMCs)
	{
		Subsystem->AddMappingContext(CurrentContext, 0);
	}
	InventoryComponent = FindComponentByClass<UInventoryComponent>();
	InventoryComponent->OnToggleHUD.AddDynamic(this, &APlayerCharacterController::ToggleHUD);
	DrawComponent = FindComponentByClass<UDrawComponent>();
	DrawComponent->OnToggleHUD.AddDynamic(this, &APlayerCharacterController::ToggleHUD);
	
	CreateHUDWidget();
	
	SetTimerPlayerPositionUpdate();
}

void APlayerCharacterController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	GetPlayerCharacter();
}

void APlayerCharacterController::SetTimerPlayerPositionUpdate()
{
	FTimerHandle PositionUpdateTimer;
	GetWorld()->GetTimerManager().SetTimer(PositionUpdateTimer, [this] ()
	{
		const APawn* Pawn = GetPawn();
		if (!IsValid(Pawn)) return;
		
		const FVector2D Location = FVector2D(Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y);
		const float Angle = Pawn->GetActorRotation().Yaw;
		const FGameplayTag Layer = Pawn->GetActorLocation().Z > 12000.f ? Layer::Upper : Pawn->GetActorLocation().Z < 0.f ? Layer::Lower : Layer::Middle;
		if (!PreviousLayer.MatchesTagExact(Layer))
		{
			OnPlayerLayerUpdated.Broadcast(Layer);
			PreviousLayer = Layer;
		}
		OnPlayerPositionUpdated.Broadcast(Location, Angle);
	}, .2f, true);
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &APlayerCharacterController::PrimaryInteract);
	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Completed, this, &APlayerCharacterController::StopFiring);
	EnhancedInputComponent->BindAction(SecondaryInteractAction, ETriggerEvent::Started, this, &APlayerCharacterController::SecondaryInteract, true);
	EnhancedInputComponent->BindAction(SecondaryInteractAction, ETriggerEvent::Completed, this, &APlayerCharacterController::SecondaryInteract, false);
	EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &APlayerCharacterController::ToggleInventory);
	EnhancedInputComponent->BindAction(ToggleGloveAction, ETriggerEvent::Started, this, &APlayerCharacterController::ToggleGlove);
	EnhancedInputComponent->BindAction(ToggleGloveAction, ETriggerEvent::Completed, this, &APlayerCharacterController::ToggleGlove);
	EnhancedInputComponent->BindAction(EquipmentAction_1, ETriggerEvent::Started, this, &APlayerCharacterController::ToggleWeapon, 1);
	EnhancedInputComponent->BindAction(EquipmentAction_2, ETriggerEvent::Started, this, &APlayerCharacterController::ToggleWeapon, 2);
	
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacterController::DoJumpStart);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacterController::DoJumpEnd);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Look);
	EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Look);
	EnhancedInputComponent->BindAction(KnowledgeAction, ETriggerEvent::Started, this, &APlayerCharacterController::ToggleKnowledgeLog);
}

void APlayerCharacterController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (GetPlayerCharacter())
	{
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
	}
}

void APlayerCharacterController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetPlayerCharacter())
	{
		PlayerCharacter->AddControllerYawInput(LookAxisVector.X);
		PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacterController::DoJumpStart()
{
	if (GetPlayerCharacter()) PlayerCharacter->Jump();
}

void APlayerCharacterController::DoJumpEnd()
{
	if (GetPlayerCharacter()) PlayerCharacter->StopJumping();
}

void APlayerCharacterController::StartFiring()
{
	if (GetPlayerCharacter()) PlayerCharacter->StartActive();
}

void APlayerCharacterController::StopFiring()
{
	if (GetPlayerCharacter()) PlayerCharacter->StopActive();
}

void APlayerCharacterController::ToggleWeapon(int32 Index)
{
	if (GetPlayerCharacter())
	{
		FGameplayTag EquipmentType = FGameplayTag::EmptyTag;
		switch (Index)
		{
			case 1: EquipmentType = Item::Equipment::Weapons::Gun;
			break;
			case 2: EquipmentType= Item::Equipment::Weapons::Scanner;
			break;
		default: ;
		}
		PlayerCharacter->ToggleActive(EquipmentType);
		HUDWidget->UpdateActiveEquipmentWidget(EquipmentType, false);
	}
}

void APlayerCharacterController::ToggleKnowledgeLog()
{
	HUDWidget->ToggleKnowledgeLog();
}

void APlayerCharacterController::PrimaryInteract()
{
	if (bAimDownSight)
	{
		StartFiring();
		return;
	}
	
	if (!ThisActor.IsValid()) return;
	
	UItemComponent* ItemComponent = ThisActor->FindComponentByClass<UItemComponent>();
	if (IsValid(ItemComponent) && InventoryComponent.IsValid())
	{
		InventoryComponent->TryAddItem(ItemComponent);
		return;
	}

	UInteractionComponent* InteractionComponent = ThisActor->FindComponentByClass<UInteractionComponent>();
	if (IsValid(InteractionComponent))
	{
		InteractionComponent->Interact();
	}
	
	ULockComponent* LockComponent = ThisActor->FindComponentByClass<ULockComponent>();
	if (IsValid(LockComponent) || !DrawComponent.IsValid())
	{
		const FGameplayTag LockType = LockComponent->GetLockType();
		if (!LockType.IsValid())
		{
			LockComponent->Unlock();
			return;
		}
		if (InventoryComponent->CheckItemOfTypeAndAmount(LockType, 1))
		{
			InventoryComponent->Server_ConsumeItemOfTypAndAmount(LockType, 1);
			LockComponent->Unlock();
			return;
		}
	}

	if (ThisComponent.IsValid())
	{
		UDigitComponent* DigitComponent = Cast<UDigitComponent>(ThisComponent);
		if (IsValid(DigitComponent))
		{
			DigitComponent->PressInput();
			return;
		}
	}
	
	UDoorComponent* DoorComponent = ThisActor->FindComponentByClass<UDoorComponent>();
	if (!IsValid(DoorComponent) || !DrawComponent.IsValid()) return;
	
	if (InventoryComponent->IsMenuOpen()) InventoryComponent->ToggleInventoryMenu();
	DrawComponent->TryDrawing(DoorComponent);
}

void APlayerCharacterController::SecondaryInteract(bool bADS)
{
	bAimDownSight = bADS;
}

void APlayerCharacterController::CreateHUDWidget()
{
	if (!IsLocalController()) return;

	HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);

	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void APlayerCharacterController::ToggleInventory()
{
	if (DrawComponent.IsValid() && DrawComponent->IsDrawingBoardOpen()) return;
	if (!InventoryComponent.IsValid() ) return;
	InventoryComponent->ToggleInventoryMenu();
	if (InventoryComponent->IsMenuOpen())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		HUDWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void APlayerCharacterController::ToggleHUD(bool bShow)
{
	HUDWidget->ToggleHUD(bShow);
}

void APlayerCharacterController::CalculateAOPitch()
{
	if (!IsValid(GetPawn())) return;
	
	AO_Pitch = GetPawn()->GetBaseAimRotation().Pitch;
	if(AO_Pitch > 90.f && !GetPawn()->IsLocallyControlled())
	{
		// Mapping pitch from [270, 360) to [-90, 0)
		const FVector2D InRange(270.f, 360.f);
		const FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void APlayerCharacterController::ToggleGlove(const FInputActionValue& Value)
{
	bGloveRaised = Value.Get<bool>();
}

void APlayerCharacterController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;

	FVector TraceStart;
	FVector Forward;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward)) return;

	const FVector TraceEnd = TraceStart + Forward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, InteractionTraceChannel);

	LastActor = ThisActor;
	LastComponent = ThisComponent;
	ThisActor = HitResult.GetActor();
	ThisComponent = HitResult.GetComponent();

	if (!ThisActor.IsValid())
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
	}

	if (ThisActor == LastActor) return;

	if (ThisActor.IsValid())
	{		
		UInteractionComponent* InteractionComponent = ThisActor->FindComponentByClass<UInteractionComponent>();
		if (!IsValid(InteractionComponent)) return;
		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(InteractionComponent->GetInteractionMessage());
	}

	if (LastActor.IsValid())
	{
		
	}
}

APlayerCharacter* APlayerCharacterController::GetPlayerCharacter()
{
	if (!PlayerCharacter.IsValid())
	{
		PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	}
	return PlayerCharacter.Get();
}