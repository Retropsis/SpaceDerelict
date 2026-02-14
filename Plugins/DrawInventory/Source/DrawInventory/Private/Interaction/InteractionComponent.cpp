// Retropsis 2026

#include "Interaction/InteractionComponent.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InteractionMessage = FString("E - Pick Up");
}

