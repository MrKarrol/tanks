#include "THealthComponent.h"


void UTHealthComponent::SetHealth(float NewHealth)
{
	if (Health > 0)
	{
		float OldHealth = Health;
		Health = FMath::Max(0.f, NewHealth);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Health %f OldHealth %f"), Health, OldHealth));
		OnHealthChangedDelegate.Broadcast(Health, OldHealth);
		if (FMath::IsNearlyZero(Health))
		{
			OnDieDelegate.Broadcast();
		}
		else if (Health < OldHealth)
		{
			OnDamageDelegate.Broadcast();
		}
	}
}

float UTHealthComponent::GetHealth() const
{
	return Health;
}
