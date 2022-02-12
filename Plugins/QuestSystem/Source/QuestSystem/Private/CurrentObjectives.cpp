// Fill out your copyright notice in the Description page of Project Settings.


#include "CurrentObjectives.h"

#include "Quest.h"
#include "ObjectiveWidget.h"
#include "Components/VerticalBox.h"

void UCurrentObjectives::SetCurrentObjectives(AQuest* Quest)
{
	if (ObjectivesList && ObjectiveWidgetClass)
	{
		ObjectivesList->ClearChildren();
		for (const auto Objective : Quest->GetObjectives())
		{
			const auto ObjectiveWidget = CreateWidget<UObjectiveWidget>(this, ObjectiveWidgetClass);
			ObjectiveWidget->Init(Objective);
			ObjectivesList->AddChildToVerticalBox(ObjectiveWidget);
		}
	}
}
