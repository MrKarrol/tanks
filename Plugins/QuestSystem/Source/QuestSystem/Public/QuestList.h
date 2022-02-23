// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestList.generated.h"

class UQuestListComponent;
class UScrollBox;
class UQuestDescription;
class UQuestListEntry;
class AQuest;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestList : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UQuestListComponent * QuestList);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UScrollBox * StoryQuestsList;

	UPROPERTY(meta = (BindWidgetOptional))
	UScrollBox * SideQuestsList;

	UPROPERTY(meta = (BindWidgetOptional))
	UQuestDescription * ActiveQuestDescription;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UQuestListEntry> QuestListEntryClass;

	UPROPERTY()
	TMap<AQuest *, UQuestListEntry *> QuestEntries;

	void OnActiveQuestChanged(AQuest * ActiveQuest);
	
};
