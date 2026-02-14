// Retropsis 2026

#include "World/Level/Door/DoorComponent.h"

UDoorComponent::UDoorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InteractionMessage = FString("E - Open Door");
}

