// Retropsis 2026

#include "World/Actor/Display.h"
#include "Data/KnowledgeData.h"
#include "Game/DerelictGameMode.h"
#include "Interaction/KnowledgeComponent.h"

ADisplay::ADisplay()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));
	DisplayMesh->SetupAttachment(Root);
	DisplayMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	KnowledgeComponent_X = CreateDefaultSubobject<UKnowledgeComponent>("KnowledgeComponent_X");
	KnowledgeComponent_X->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	KnowledgeComponent_X->SetupAttachment(DisplayMesh);
	
	KnowledgeComponent_Y = CreateDefaultSubobject<UKnowledgeComponent>("KnowledgeComponent_Y");
	KnowledgeComponent_Y->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	KnowledgeComponent_Y->SetupAttachment(DisplayMesh);
}

void ADisplay::SetupDisplay(const FIntPoint& Coordinates)
{
	for (const FName& SlotName : DisplayMesh->GetMaterialSlotNames())
	{
		if (SlotName.IsEqual(FName("Display_X")) && Numerics.IsValidIndex(Coordinates.X))
		{
			DisplayMesh->SetMaterialByName(FName("Display_X"), Numerics[Coordinates.X]);
			SetupKnowledgeComponent(KnowledgeComponent_X, Coordinates.X);
		}
		if (SlotName.IsEqual(FName("Display_Y")) && Numerics.IsValidIndex(Coordinates.Y))
		{
			DisplayMesh->SetMaterialByName(FName("Display_Y"), Numerics[Coordinates.Y]);
			SetupKnowledgeComponent(KnowledgeComponent_Y, Coordinates.Y);
		}
	}
}

void ADisplay::SetupKnowledgeComponent(UKnowledgeComponent* KnowledgeComponent, const int32 Integer) const
{
	ADerelictGameMode* DerelictGameMode = Cast<ADerelictGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(DerelictGameMode) && IsValid(DerelictGameMode->GetKnowledgeData()))
	{
		FSymbolData SymbolData = DerelictGameMode->GetKnowledgeData()->GetNumericSymbolDataByInt(Integer);
		if (!IsValid(KnowledgeComponent)) return;
						
		FKnowledgeImage* KnowledgeImage = KnowledgeComponent->GetFragmentOfTypeMutable<FKnowledgeImage>();
		FKnowledgeText* KnowledgeText = KnowledgeComponent->GetFragmentOfTypeMutable<FKnowledgeText>();
		if (!KnowledgeImage || !KnowledgeText) return;
						
		KnowledgeImage->SetImage(SymbolData.Texture);
		KnowledgeText->SetText(SymbolData.Text);
		KnowledgeComponent->SetKnowledgeTag(SymbolData.KnowledgeTag);
	}
}
