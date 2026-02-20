// Retropsis 2026

#include "World/Level/Switch/SwitchComponent.h"

USwitchComponent::USwitchComponent()
{
	SetInteractionMessage(InteractionMessage);
}

void USwitchComponent::Interact()
{
	ToggleSwitch(bIsOn);
}

void USwitchComponent::ToggleSwitch(const bool bOn)
{
	bIsOn = !bOn;
	OnSwitch.Broadcast(bIsOn);
}
