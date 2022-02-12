// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"

class UObjective;
class UCheckBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UObjective * InitObjective);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UCheckBox * CompletionCheck;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock * DescriptionText;

private:
	UPROPERTY()
	UObjective * Objective;
	
};
