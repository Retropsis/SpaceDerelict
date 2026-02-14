// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUD/InfoMessage.h"
#include "Components/TextBlock.h"

void UInfoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Text_Message->SetText(FText::GetEmpty());
	HideMessage();
}

void UInfoMessage::SetMessage(const FText& Message)
{
	Text_Message->SetText(Message);

	if (!bIsMessageActive)
	{
		ShowMessage();
	}
	bIsMessageActive = true;
	
	TWeakObjectPtr<UInfoMessage> WeakThis(this);
	GetWorld()->GetTimerManager().SetTimer(MessageTimer, [WeakThis] ()
	{
		
		if (const auto StrongThis = WeakThis.Pin())
		{
		   StrongThis->HideMessage();
		   StrongThis->bIsMessageActive = false;
		}
	}, MessageLifeTime, false);
}
