// Retropsis 2026

#include "DrawManagement/Room/RoomActor.h"
#include "Data/DestinationData.h"
#include "DrawManagement/Room/ItemSpawner.h"
#include "DrawManagement/Utility/DrawingUtility.h"
#include "World/Level/Door/Door.h"
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
		ADoor* Door = GetWorld()->SpawnActor<ADoor>(DoorClass);
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

		Door->SwitchDoorState(Availability.DoorState);

		if (Availability.DoorState == EDoorState::Sealed) continue;

		const FString NewComponentLabel = FString::Printf(TEXT("AC_DoorComponent_%s"), *Availability.Socket.ToString());
		UDoorComponent* DoorComponent = NewObject<UDoorComponent>(Door, DoorComponentClass, FName(NewComponentLabel));
		if (!IsValid(DoorComponent)) return;
		DoorComponent->RegisterComponent();
		Door->AddInstanceComponent(DoorComponent);
		DoorComponent->SetDoorState(Availability.DoorState);
		DoorComponent->SetRoomIndex(Result.RoomIndex);
		DoorComponent->SetDestinationIndex(Availability.DestinationIndex);
		if (Availability.DoorState == EDoorState::Locked) DoorComponent->SetInteractionMessageToLocked();
		const int32 DestinationYaw = FMath::Modulo(static_cast<int32>(Yaw) + Result.DestinationYaw, 360);
		DoorComponent->SetRoomYaw(DestinationYaw);
		
		DoorComponentToSockets.Add(Availability.Socket, DoorComponent);
		// UE_LOG(LogTemp, Warning, TEXT("Constructing Door with destination index %d and Yaw %d, at socket %s"), Availability.DestinationIndex, DestinationYaw, *Availability.Socket.ToString());
	}
}

UDoorComponent* ARoomActor::GetDoorComponentBySocket(const FName& Socket)
{
	if (!Socket.IsValid() || !DoorComponentToSockets.Contains(Socket)) return nullptr;

	return DoorComponentToSockets[Socket];
}

TArray<FTransform> ARoomActor::GetAvailableSpawnerTransforms() const
{
	TArray<FTransform> SpawnerTransforms;
	TArray<UActorComponent*> Components;
	GetComponents(UItemSpawner::StaticClass(), Components);
	for (UActorComponent* Component : Components)
	{
		USceneComponent* SceneComponent = Cast<USceneComponent>(Component);
		if (IsValid(SceneComponent)) SpawnerTransforms.Add(SceneComponent->GetComponentTransform());
	}
	return SpawnerTransforms;
}

