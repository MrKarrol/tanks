#pragma once


#include "Components/ActorComponent.h"
#include "THealthComponent.generated.h"

	
UCLASS()
class UTHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float/*NewHealth*/, float/*OldHealth*/);
	DECLARE_MULTICAST_DELEGATE(FOnDieDelegate);
	DECLARE_MULTICAST_DELEGATE(FOnDamageDelegate);
	DECLARE_MULTICAST_DELEGATE(FOnHealDelegate);
public:
	UTHealthComponent();

	void SetHealth(float Health);
	float GetHealth() const;

	FOnHealthChangedDelegate OnHealthChangedDelegate;
	FOnDieDelegate OnDieDelegate;
	FOnHealDelegate OnHealDelegate;
	FOnDamageDelegate OnDamageDelegate;

protected:
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

private:
	float mCurrentHealth;

};