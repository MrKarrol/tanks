// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tanks/Widgets/TWidget.h"
#include "THealthBarInGame.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class TANKS_API UTHealthBarInGame : public UTWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

private:
	void SetupHealth(float NewHealth, float OldHealth);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UProgressBar *HealthBar;

private:
	float m_max_health;
	
};
