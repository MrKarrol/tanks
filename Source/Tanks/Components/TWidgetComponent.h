// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class TANKS_API UTWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:

	virtual void InitWidget() override;
	
	UTWidgetComponent();
	
};