#pragma once


#include "Components/ActorComponent.h"
#include "THealthComponent.generated.h"

	
UCLASS()
class UTHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float/*NewHealth*/, float/*OldHealth*/);
	DECLARE_MULTICAST_DELEGATE(FOnDieDelegate);
public:
	void SetHealth(float Health);
	float GetHealth() const;

	FOnHealthChangedDelegate OnHealthChangedDelegate;
	FOnDieDelegate OnDieDelegate;

protected:
	UPROPERTY(EditAnywhere)
	float Health = 100.f;

};