// Retropsis 2026

#include "Equipment/Tool/KnowledgeScanner.h"
#include "Equipment/Weapon/WeaponInterface.h"
#include "CableComponent.h"
#include "Interaction/KnowledgeComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayerCharacterController.h"
#include "Widget/HUD/HUDWidget.h"

AKnowledgeScanner::AKnowledgeScanner()
{
	LinkComponent = CreateDefaultSubobject<UCableComponent>("LinkComponent");
	LinkComponent->SetupAttachment(RootComponent);
	LinkComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LinkComponent->CableWidth = 2.f;
	LinkComponent->SetVisibility(false);
}

void AKnowledgeScanner::InitializeEquipment()
{	
	if (const APlayerCharacterController* OwnerController = Cast<APlayerCharacterController>(GetOwner()))
	{
		if (UHUDWidget* HUDWidget = OwnerController->GetHUDWidget())
		{
			OnScanProgress.AddDynamic(HUDWidget, &UHUDWidget::OnScanProgress);
			OnScanComplete.AddDynamic(HUDWidget, &UHUDWidget::OnScanComplete);
		}
		if (APawn* OwnerPawn = Cast<APawn>(OwnerController->GetPawn()))
		{
			EquipmentOwner = Cast<IWeaponInterface>(OwnerPawn);
			PawnOwner = Cast<APawn>(OwnerPawn);
		}
	}
}

void AKnowledgeScanner::StartActive()
{
	bIsScanning = true;

	const float TimeSinceLastShot = GetWorld()->GetTimeSeconds() - TimeOfLastScan;
	if (TimeSinceLastShot > ScanRate)
	{
		Scan();
	}
}

void AKnowledgeScanner::StopActive()
{
	bIsScanning = false;
	LinkComponent->SetVisibility(false);
	GetWorld()->GetTimerManager().ClearTimer(ScanTimer);
	ScanTimer.Invalidate();
	OnScanProgress.Broadcast(-1.f);
	LinkComponent->bAttachEnd = true;
}

void AKnowledgeScanner::BeginPlay()
{
	Super::BeginPlay();
	LinkComponent->SetRelativeLocation(EquipmentMesh->GetSocketLocation(MuzzleSocketName));
}

void AKnowledgeScanner::Scan()
{
	if (!bIsScanning)
	{
		return;
	}
	
	FireScanLink(EquipmentOwner->GetActiveTargetLocation());
	TimeOfLastScan = GetWorld()->GetTimeSeconds();

	StartScanProgress(TimeToComplete);

	GetWorld()->GetTimerManager().SetTimer(ScanTimer, [this] ()
	{
		if (DeferredDetachmentTime > 0.0f)
		{
			// GetWorld()->GetTimerManager().SetTimer(DeferredUnlinkTimer, this, &AKnowledgeScanner::OnDeferredUnlink, DeferredUnlinkTime, false);
			OnDeferredUnlink();
			GetWorld()->GetTimerManager().SetTimer(DeferredDetachmentTimer, this, &AKnowledgeScanner::OnDeferredDetachment, DeferredDetachmentTime, false);
		}
		else
		{
			StopActive();
		}
	}, TimeToComplete, false);

	// MakeNoise(ShotLoudness, PawnOwner, PawnOwner->GetActorLocation(), ShotNoiseRange, ShotNoiseTag);
}

void AKnowledgeScanner::FireScanLink(const FVector& TargetLocation)
{
	KnowledgeComponent.Reset();
	LinkComponent->SetVisibility(true);
	FTransform TargetTransform = CalculateTargetTransform(TargetLocation);

	FHitResult Hit;
	const FVector Start = EquipmentMesh->GetSocketLocation(MuzzleSocketName);
	const FVector End = Start + ((TargetLocation - Start).GetSafeNormal() * 5000.f);
	UKismetSystemLibrary::DrawDebugSphere(this, End, 15.f, 12, FLinearColor::Green, 5.f);;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	if (Hit.bBlockingHit && IsValid(Hit.GetComponent()))
	{
		KnowledgeComponent = Cast<UKnowledgeComponent>(Hit.GetComponent());
		if (KnowledgeComponent.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Scanning Component %s"), *KnowledgeComponent->GetName());
			LinkComponent->SetAttachEndToComponent(Hit.GetComponent());
			LinkComponent->EndLocation = FVector(Hit.ImpactPoint - Hit.GetComponent()->GetComponentLocation());
		}
		else
		{
			ScanLinkFailed();
		}
	}
	else
	{
		ScanLinkFailed();
	}
}

void AKnowledgeScanner::ScanComplete()
{
	OnScanComplete.Broadcast(KnowledgeComponent.Get());
}

void AKnowledgeScanner::ScanLinkFailed()
{
	LinkComponent->SetAttachEndToComponent(EquipmentMesh, MuzzleSocketName);
	LinkComponent->EndLocation = FVector(5000.f, 0.f, 0.f);
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, LinkComponent->EndLocation.ToString());
	if (DeferredDetachmentTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(DeferredUnlinkTimer, this, &AKnowledgeScanner::OnDeferredUnlink, DeferredUnlinkTime, false);
		GetWorld()->GetTimerManager().SetTimer(DeferredDetachmentTimer, this, &AKnowledgeScanner::OnDeferredDetachment, DeferredDetachmentTime, false);
	}
	else
	{
		StopActive();
	}
}

void AKnowledgeScanner::OnDeferredDetachment()
{
	StopActive();
}

void AKnowledgeScanner::OnDeferredUnlink()
{
	LinkComponent->bAttachEnd = false;
}

FTransform AKnowledgeScanner::CalculateTargetTransform(const FVector& TargetLocation) const
{
	const FVector MuzzleLoc = EquipmentMesh->GetSocketLocation(MuzzleSocketName);
	const FVector SpawnLoc = MuzzleLoc + ((TargetLocation - MuzzleLoc).GetSafeNormal() * MuzzleOffset);
	const FRotator AimRot = UKismetMathLibrary::FindLookAtRotation(SpawnLoc, TargetLocation + (UKismetMathLibrary::RandomUnitVector() * AimVariance));
	return FTransform(AimRot, SpawnLoc, FVector::OneVector);
}
