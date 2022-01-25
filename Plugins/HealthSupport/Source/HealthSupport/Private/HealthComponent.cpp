// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	mCurrentHealth = MaxHealth;
}

void UHealthComponent::SetHealth(float NewHealth)
{
	if (mCurrentHealth > 0.f)
	{
		float OldHealth = mCurrentHealth;
		mCurrentHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
		OnHealthChangedDelegate.Broadcast(mCurrentHealth, OldHealth);
		if (FMath::IsNearlyZero(mCurrentHealth))
		{
			OnDieDelegate.Broadcast();
		}
		else if (mCurrentHealth < OldHealth)
		{
			OnDamageDelegate.Broadcast();
		}
		else if (mCurrentHealth > OldHealth)
		{
			OnHealDelegate.Broadcast();
		}
	}
}

float UHealthComponent::GetHealth() const
{
	return mCurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}


