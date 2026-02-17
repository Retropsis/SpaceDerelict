// Retropsis 2026

#include "World/Level/Door/DoorComponent.h"
#include "World/Level/Door/Door.h"

UDoorComponent::UDoorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InteractionMessage = FString("E - Open Door");
	DoorState = EDoorState::None;
}

void UDoorComponent::ToggleDoor(const bool bOpen)
{	
	ADoor* Door = Cast<ADoor>(GetOwner());
	if (IsValid(Door))
	{
		Door->ToggleDoor(bOpen);
	}
	if (bOpen) DestroyComponent();
}

void UDoorComponent::Unlock()
{
	DoorState = EDoorState::Opened;
	ADoor* Door = Cast<ADoor>(GetOwner());
	if (IsValid(Door))
	{
		Door->SwitchDoorState(DoorState);
	}
}

void UDoorComponent::SwitchDoorState(const EDoorState NewState)
{
	DoorState = NewState;
	ADoor* Door = Cast<ADoor>(GetOwner());
	if (IsValid(Door))
	{
		Door->SwitchDoorState(DoorState);
	}
}

void UDoorComponent::SetInteractionMessageToLocked()
{
	InteractionMessage = FString("E - Unlock Door");
}

