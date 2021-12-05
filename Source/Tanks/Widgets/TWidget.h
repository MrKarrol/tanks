// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API UTWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DELEGATE(FOnNeedToRemove);

public:
	FOnNeedToRemove OnNeedToRemove;
	
};
