// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestListComponent.h"

#include "Quest.h"
#include "CurrentObjectives.h"


void UQuestListComponent::AddQuest(AQuest* Quest)
{
	if (! Quest)
		return;

	AcceptedQuests.AddUnique(Quest);
	Quest->TakeQuest(GetOwner());
	Quest->OnQuestStatusUpdated.AddLambda([this](AQuest * Quest)
	{
		if (ActiveQuest == Quest)
		{
			if (OnActiveQuestChanged.IsBound())
				OnActiveQuestChanged.Broadcast(Quest);
		}
	});
}

const TArray<AQuest*>& UQuestListComponent::GetQuests() const
{
	return AcceptedQuests;
}

AQuest* UQuestListComponent::GetActiveQuest() const
{
	return ActiveQuest;
}

void UQuestListComponent::SetActiveQuest(AQuest* Quest)
{
	if (AcceptedQuests.Contains(Quest))
	{
		ActiveQuest = Quest;
		if (OnActiveQuestChanged.IsBound())
		{
			OnActiveQuestChanged.Broadcast(Quest);
		}
	}
}

void UQuestListComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentObjectivesWidgetClass)
	{
		const auto CurrentObjectivesWidget = CreateWidget<UCurrentObjectives>(
			GetWorld(), CurrentObjectivesWidgetClass);
		OnActiveQuestChanged.AddUObject(CurrentObjectivesWidget,
			&UCurrentObjectives::SetCurrentObjectives);
		CurrentObjectivesWidget->AddToViewport();
	}
}


// Called every frame
void UQuestListComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

