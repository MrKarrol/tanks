// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Tanks/Widgets/TWidget.h"
#include "TMediaPlayer.generated.h"

class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class TANKS_API UTMediaPlayer : public UTWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnBrowseClicked();

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * Browse;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage * Preview;
	
};
