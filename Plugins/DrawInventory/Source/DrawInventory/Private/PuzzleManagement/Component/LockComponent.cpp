// Retropsis 2026


#include "PuzzleManagement/Component/LockComponent.h"


ULockComponent::ULockComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULockComponent::Unlock()
{
	OnUnlock.Broadcast();
	DestroyComponent();
}

