// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDialog.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "QuestDescription.h"

void UQuestDialog::NativeConstruct()
{
	Super::NativeConstruct();

	if (RejectButton)
		RejectButton->OnReleased.AddDynamic(this, &UQuestDialog::RejectQuest);

	if (AcceptButton)
		AcceptButton->OnReleased.AddDynamic(this, &UQuestDialog::AcceptQuest);

	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController);
	PlayerController->bShowMouseCursor = true;
}

void UQuestDialog::Init(AQuest* Quest)
{
	if (Description)
		Description->Init(Quest);
}

void UQuestDialog::RejectQuest()
{
	HideDialog();
}

void UQuestDialog::AcceptQuest()
{
	HideDialog();
	if (OnQuestAccepted.IsBound())
		OnQuestAccepted.Execute();
}

void UQuestDialog::HideDialog()
{
	RemoveFromViewport();

	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	PlayerController->bShowMouseCursor = true; // for tanks only

	if (OnQuestQuited.IsBound())
		OnQuestQuited.Execute();
}
