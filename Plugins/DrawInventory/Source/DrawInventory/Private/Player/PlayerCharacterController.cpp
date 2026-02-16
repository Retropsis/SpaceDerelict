// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacterController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawManagement/Component/DrawComponent.h"
#include "InventoryManagement/Component/InventoryComponent.h"
#include "Interaction/InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/HUD/HUDWidget.h"
#include "World/Level/Door/DoorComponent.h"

APlayerCharacterController::APlayerCharacterController()
{
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.0;
	InteractionTraceChannel = ECC_GameTraceChannel2;
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceForItem();
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
	DrawComponent = FindComponentByClass<UDrawComponent>();
	CreateHUDWidget();
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &APlayerCharacterController::PrimaryInteract);
	EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &APlayerCharacterController::ToggleInventory);
}

void APlayerCharacterController::PrimaryInteract()
{
	if (!ThisActor.IsValid()) return;

	UItemComponent* ItemComponent = ThisActor->FindComponentByClass<UItemComponent>();
	if (IsValid(ItemComponent) && InventoryComponent.IsValid())
	{
		InventoryComponent->TryAddItem(ItemComponent);
		return;
	}
	
	UDoorComponent* DoorComponent = ThisActor->FindComponentByClass<UDoorComponent>();
	if (!IsValid(DoorComponent) || !DrawComponent.IsValid()) return;

	DrawComponent->TryDrawing(DoorComponent);
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
	ThisActor = HitResult.GetActor();

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

void APlayerCharacterController::ToggleDrawingBoard()
{
	
}
