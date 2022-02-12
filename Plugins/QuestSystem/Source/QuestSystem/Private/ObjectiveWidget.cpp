// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveWidget.h"

#include "Objective.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"

void UObjectiveWidget::Init(UObjective* InitObjective)
{
	Objective = InitObjective;

	if (Objective)
	{
		if (CompletionCheck)
			CompletionCheck->SetIsChecked(Objective->bIsCompleted);
		if (DescriptionText)
			DescriptionText->SetText(Objective->Description);
	}
}
