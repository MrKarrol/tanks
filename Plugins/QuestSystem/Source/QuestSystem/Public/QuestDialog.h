// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDialog.generated.h"


class UQuestDescription;

class AQuest;
class UButton;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void Init(AQuest * Quest);

protected:
	UFUNCTION()
	void RejectQuest();

	UFUNCTION()
	void AcceptQuest();

	void HideDialog();

public:
	FSimpleDelegate OnQuestAccepted;
	FSimpleDelegate OnQuestQuited;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UQuestDescription * Description;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton * RejectButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton * AcceptButton;

};
