// Retropsis 2026

#include "Widget/MainMenu/MainMenu.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnClicked.AddDynamic(this, &ThisClass::UMainMenu::RestartGame);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::UMainMenu::QuitGame);
}

void UMainMenu::RestartGame()
{
	UGameplayStatics::OpenLevel(GetOwningPlayer(), GetWorld()->GetCurrentLevel()->GetFName());
}

void UMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
