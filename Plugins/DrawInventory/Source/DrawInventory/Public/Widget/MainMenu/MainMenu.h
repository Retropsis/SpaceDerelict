// Retropsis 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;


UCLASS()
class DRAWINVENTORY_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

private:	
	UFUNCTION()
	void RestartGame();
	
	UFUNCTION()
	void QuitGame();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
};
