// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestList.h"

#include "QuestListComponent.h"
#include "Quest.h"
#include "QuestListEntry.h"
#include "Components/ScrollBox.h"
#include "QuestDescription.h"


void UQuestList::Init(UQuestListComponent* QuestList)
{
	if (QuestList && QuestListEntryClass)
	{
		for (const auto Quest : QuestList->GetQuests())
		{
			const auto Scroll = Quest->IsStoryQuest() ?
				StoryQuestsList : SideQuestsList;
			if (Scroll)
			{
				const auto QuestEntry =
					CreateWidget<UQuestListEntry>(this, QuestListEntryClass);
				QuestEntry->SetQuestText(Quest->GetName());
				QuestEntry->OnQuestChosen.BindUObject(QuestList,
					&UQuestListComponent::SetActiveQuest, Quest);
				QuestEntries.Add(Quest, QuestEntry);
				Scroll->AddChild(QuestEntry);
			}
		}
		OnActiveQuestChanged(QuestList->GetActiveQuest());
		QuestList->OnActiveQuestChanged.AddUObject(
			this, &UQuestList::OnActiveQuestChanged);
	}
}

void UQuestList::OnActiveQuestChanged(AQuest* ActiveQuest)
{
	for (const auto Pair : QuestEntries)
	{
		Pair.Value->SetIsSelected(Pair.Key == ActiveQuest);
	}
	if (ActiveQuestDescription)
	{
		ActiveQuestDescription->Init(ActiveQuest);
		ActiveQuestDescription->SetVisibility(ActiveQuest ?
			ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
