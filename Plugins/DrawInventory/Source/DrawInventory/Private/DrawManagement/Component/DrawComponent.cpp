// Retropsis 2026

#include "DrawManagement/Component/DrawComponent.h"

#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Widget/DrawInventory/DrawingBoard.h"

UDrawComponent::UDrawComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bDrawingBoardMenuOpen = false;
}

void UDrawComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(ThisClass, InventoryList);
}

void UDrawComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

void UDrawComponent::BeginPlay()
{
	Super::BeginPlay();	
	ConstructDrawingBoard();
}

void UDrawComponent::ConstructDrawingBoard()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("DrawComponent should have PlayerController as Owner."));
	if (!OwningController->IsLocalController()) return;

	DrawingBoard = CreateWidget<UDrawingBoard>(OwningController.Get(),DrawingBoardClass);
	DrawingBoard->AddToViewport();
	CloseDrawingBoard();
}

void UDrawComponent::OpenDrawingBoard()
{
	
}

void UDrawComponent::CloseDrawingBoard()
{
	
}

void UDrawComponent::TryDrawing()
{
	
}

