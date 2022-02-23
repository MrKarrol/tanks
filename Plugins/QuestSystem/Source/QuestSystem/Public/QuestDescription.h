// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDescription.generated.h"


class AQuest;
class UTextBlock;
class UScrollBox;
class UObjectiveWidget;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestDescription : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(AQuest * Quest);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock * NameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox * ObjectivesList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock * DescriptionText;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UObjectiveWidget> ObjectiveWidgetClass;
};
