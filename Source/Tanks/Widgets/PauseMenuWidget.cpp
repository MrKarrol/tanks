// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReturnToGameBtn)
		ReturnToGameBtn->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnReturnToGameClicked);

	if(MainMenuBtn)
		MainMenuBtn->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuClicked);

	if (QuitBtn)
		QuitBtn->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);

	UGameplayStatics::SetGamePaused(this, true);
}

void UPauseMenuWidget::OnReturnToGameClicked()
{
	OnNeedToRemove.Execute();
	UGameplayStatics::SetGamePaused(this, false);
}

void UPauseMenuWidget::OnMainMenuClicked()
{
	UGameplayStatics::OpenLevel(this, *MainMenuLevel.GetAssetName());
}

void UPauseMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
