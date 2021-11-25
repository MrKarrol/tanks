#include "THealthComponent.h"


void UTHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	mCurrentHealth = MaxHealth;
}

void UTHealthComponent::SetHealth(float NewHealth)
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

float UTHealthComponent::GetHealth() const
{
	return mCurrentHealth;
}
