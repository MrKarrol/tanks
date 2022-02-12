// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDescription.h"

#include "Quest.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "ObjectiveWidget.h"


void UQuestDescription::Init(AQuest* Quest)
{
	if (! Quest)
		return;

	if (NameText)
		NameText->SetText(Quest->GetName());
	
	if (DescriptionText)
		DescriptionText->SetText(Quest->GetDescription());

	if (ObjectiveWidgetClass && ObjectivesList)
	{
		ObjectivesList->ClearChildren();
		for (const auto Objective : Quest->GetObjectives())
		{
			const auto ObjectiveWidget = CreateWidget<UObjectiveWidget>(this,
				ObjectiveWidgetClass);
			ObjectiveWidget->Init(Objective);
			ObjectivesList->AddChild(ObjectiveWidget);
		}
	}
}
