// Retropsis 2026

#include "Widget/MainMenu/MainMenu.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayerCharacterController.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnClicked.AddDynamic(this, &ThisClass::UMainMenu::RestartGame);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::UMainMenu::QuitGame);
	AzertyButton->OnClicked.AddDynamic(this, &ThisClass::UMainMenu::SetDefaultMappingToAzerty);
	QwertyButton->OnClicked.AddDynamic(this, &ThisClass::UMainMenu::SetDefaultMappingToQwerty);
}

void UMainMenu::RestartGame()
{
	if (APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetOwningPlayer()))
	{
		PlayerCharacterController->RestartGame();
	}
}

void UMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UMainMenu::SetDefaultMappingToAzerty()
{
	AzertyButton->SetIsEnabled(false);
	QwertyButton->SetIsEnabled(true);
	if (const APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetOwningPlayer()))
	{
		PlayerCharacterController->SetDefaultMappingToAzerty();
	}
}

void UMainMenu::SetDefaultMappingToQwerty()
{
	AzertyButton->SetIsEnabled(true);
	QwertyButton->SetIsEnabled(false);
	if (const APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetOwningPlayer()))
	{
		PlayerCharacterController->SetDefaultMappingToQwerty();
	}
}
