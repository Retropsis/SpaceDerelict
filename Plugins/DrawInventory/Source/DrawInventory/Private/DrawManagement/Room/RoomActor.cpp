// Retropsis 2026

#include "DrawManagement/Room/RoomActor.h"
#include "Data/DestinationData.h"
#include "DrawManagement/Utility/DrawingUtility.h"
#include "World/Level/Door/DoorComponent.h"

ARoomActor::ARoomActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

TMap<FName, FIntPoint>  ARoomActor::ConstructDestinationOffsets()
{
	UStaticMeshComponent* StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (IsValid(StaticMeshComponent))
	{
		for (const FName& Socket : StaticMeshComponent->GetAllSocketNames())
		{
			FIntPoint Offset = UDrawingUtility::GetOffsetFromSocketName(Socket);
			DestinationOffsets.Add(Socket, Offset);
			// DestinationOffsets.Add(Socket, UDrawingUtility::GetShiftedOffsetFromAngle(Offset, StaticMeshComponent->GetSocketRotation(Socket).Yaw));
		}
		return DestinationOffsets;
	}
		
	USkeletalMeshComponent* SkeletalMeshComponent = FindComponentByClass<USkeletalMeshComponent>();
	if (IsValid(SkeletalMeshComponent))
	{
		for (const FName& Socket : SkeletalMeshComponent->GetAllSocketNames())
		{
			FIntPoint Offset = UDrawingUtility::GetOffsetFromSocketName(Socket);
			DestinationOffsets.Add(Socket, Offset);
			// DestinationOffsets.Add(Socket, UDrawingUtility::GetShiftedOffsetFromAngle(Offset, SkeletalMeshComponent->GetSocketRotation(Socket).Yaw));
		}
	}
	return DestinationOffsets;
}

void ARoomActor::ConstructDoors(const FDestinationAvailabilityResult& Result)
{
	checkf(DoorComponentClass, TEXT("Please fill in DoorComponentClass In RoomActor."));
	checkf(DoorClass, TEXT("Please fill in DoorClass In RoomActor."));
	
	// UE_LOG(LogTemp, Warning, TEXT("Constructing Doors for room index %d"), Result.RoomIndex);
	for (const FDestinationAvailability Availability : Result.DestinationAvailabilities)
	{
		AActor* Door = GetWorld()->SpawnActor<AActor>(DoorClass);
		const FString NewLabel = FString::Printf(TEXT("%s_%s"), *Door->GetName(), *Availability.Socket.ToString());
		Door->SetActorLabel(NewLabel);
		float Yaw{ 0 };

		UStaticMeshComponent* StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
		if (IsValid(StaticMeshComponent))
		{
			Door->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Availability.Socket);
			Yaw = StaticMeshComponent->GetSocketTransform(Availability.Socket, RTS_Component).Rotator().Yaw;
		}
		
		USkeletalMeshComponent* SkeletalMeshComponent = FindComponentByClass<USkeletalMeshComponent>();
		if (IsValid(SkeletalMeshComponent))
		{
			Door->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Availability.Socket);
			Yaw = SkeletalMeshComponent->GetSocketTransform(Availability.Socket, RTS_Component).Rotator().Yaw;
		}

		if (Availability.DoorState == EDoorState::Sealed) continue;

		UDoorComponent* DoorComponent = NewObject<UDoorComponent>(Door, DoorComponentClass, "AC_DoorComponent");
		if (!IsValid(DoorComponent)) return;
		// const FString NewLabel = EncounterSpawnData->EncounterData->GetName().Replace(TEXT("Encounter"), TEXT("SpawnPoint"));
		DoorComponent->RegisterComponent();
		Door->AddInstanceComponent(DoorComponent);
		DoorComponent->SetDoorState(Availability.DoorState);
		DoorComponent->SetRoomIndex(Result.RoomIndex);
		DoorComponent->SetDestinationIndex(Availability.DestinationIndex);
		if (Availability.DoorState == EDoorState::Locked) DoorComponent->SetInteractionMessageToLocked();
		const int32 DestinationYaw = FMath::Modulo(static_cast<int32>(Yaw) + Result.DestinationYaw, 360);
		DoorComponent->SetRoomYaw(DestinationYaw);
		// UE_LOG(LogTemp, Warning, TEXT("Constructing Door with destination index %d and Yaw %d, at socket %s"), Availability.DestinationIndex, DestinationYaw, *Availability.Socket.ToString());
	}
}

