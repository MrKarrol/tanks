#include "THealthComponent.h"


void UTHealthComponent::SetHealth(float NewHealth)
{
	if (Health > 0)
	{
		float OldHealth = Health;
		Health = FMath::Max(0.f, NewHealth);
		OnHealthChangedDelegate.Broadcast(Health, OldHealth);
		if (FMath::IsNearlyZero(Health))
		{
			OnDieDelegate.Broadcast();
		}
	}
}

float UTHealthComponent::GetHealth() const
{
	return Health;
}
