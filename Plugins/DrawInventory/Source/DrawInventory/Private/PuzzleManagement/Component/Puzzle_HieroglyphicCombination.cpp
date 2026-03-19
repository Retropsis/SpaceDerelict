// Retropsis 2026

#include "PuzzleManagement/Component/Puzzle_HieroglyphicCombination.h"
#include "PuzzleManagement/Piece/Component/PressurePlate.h"
#include "PuzzleManagement/PuzzleTags.h"
#include "Data/PuzzleData.h"
#include "DrawManagement/Room/RoomActor.h"
#include "Game/DerelictGameMode.h"
#include "World/Level/Door/Door.h"
#include "World/Level/Door/DoorComponent.h"
#include "WorldPartition/HLOD/HLODActor.h"

UPuzzle_HieroglyphicCombination::UPuzzle_HieroglyphicCombination()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPuzzle_HieroglyphicCombination::ConstructPuzzle(const FIntPoint& Coordinates)
{
	const ADerelictGameMode* DerelictGameMode = Cast<ADerelictGameMode>(GetWorld()->GetAuthGameMode());
	check(DerelictGameMode);
	
	const FHieroglyphicCombinationPattern* HieroglyphicCombinationPattern = DerelictGameMode->GetPuzzleDataOfTypeWithTag<FHieroglyphicCombinationPattern>(Puzzle::Pattern::Hieroglyphics);
	DesiredCombination = HieroglyphicCombinationPattern->GetHieroglyphicCombination();
	
	for (const TObjectPtr<AActor>& Actor : GetOwner()->GetLevel()->Actors)
	{
		if (APressurePlate* PressurePlate = Cast<APressurePlate>(Actor))
		{
			PressurePlate->OnPressurePlateActivated.AddDynamic(this, &ThisClass::OnSymbolReceived);
		}
	}

	TArray<ADoor*> Doors;
	if (const ARoomActor* RoomActor = Cast<ARoomActor>(GetOwner()))
	{
		for (TWeakObjectPtr<ADoor> Door : RoomActor->GetDoors())
		{
			if (const UDoorComponent* DoorComponent = Door->FindComponentByClass<UDoorComponent>())
			{
				if (DoorComponent->GetDoorState() == EDoorState::None || DoorComponent->GetDoorState() == EDoorState::Closed)
				{
					Doors.Add(Door.Get());
				}
			}
		}
	}
	
	const int32 DoorSelection = FMath::RandRange(0, Doors.Num() - 1);
	if (!Doors.IsValidIndex(DoorSelection) || !IsValid(Doors[DoorSelection]))
	{
		UE_LOG(LogTemp, Error, TEXT("No door was valid."));
		return;
	}
	
	ADoor* PuzzleDoor = Doors[DoorSelection];
	PuzzleDoor->UpdateDoorVisuals(HieroglyphicCombinationPattern->GetAdditionalMesh());
	if (UDoorComponent* DoorComponent = PuzzleDoor->FindComponentByClass<UDoorComponent>())
	{
		DoorComponent->SetDoorState(EDoorState::MetaLocked);
		DoorComponent->SetInteractionMessageToLocked();
		OnCombinationComplete.AddDynamic(DoorComponent, &UDoorComponent::Unlock);
	}
}

void UPuzzle_HieroglyphicCombination::OnSymbolReceived(const FGameplayTag& Symbol)
{
	InputCombination.Add(Symbol);

	bool bDesiredCombination = true;
	for (int32 i = 0; i < DesiredCombination.Num(); ++i)
	{
		if (!DesiredCombination.IsValidIndex(i) || !InputCombination.IsValidIndex(i)) return;

		if (!DesiredCombination[i].MatchesTagExact(InputCombination[i]))
		{
			bDesiredCombination = false;
		}
	}
	if (bDesiredCombination)
	{
		OnCombinationComplete.Broadcast();
	}
}