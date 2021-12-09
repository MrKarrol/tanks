// Fill out your copyright notice in the Description page of Project Settings.


#include "THealthBarInGame.h"

#include "Components/ProgressBar.h"
#include "Tanks/Core/TPawn.h"
#include "Tanks/Components/THealthComponent.h"


void UTHealthBarInGame::NativeConstruct()
{
	if (const auto owner = Cast<ATPawn>(OwningActor))
	{
		if (const auto health_component = owner->GetHealthComponent())
		{
			m_max_health = health_component->GetMaxHealth();
			health_component->OnHealthChangedDelegate.AddUObject(this, &UTHealthBarInGame::SetupHealth);
		}
	}
}

void UTHealthBarInGame::SetupHealth(float NewHealth, float OldHealth)
{
	if (HealthBar)
		HealthBar->SetPercent(NewHealth / m_max_health);
}
