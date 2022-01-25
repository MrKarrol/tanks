// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEALTHSUPPORT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float/*NewHealth*/, float/*OldHealth*/);
	DECLARE_MULTICAST_DELEGATE(FOnDieDelegate);
	DECLARE_MULTICAST_DELEGATE(FOnDamageDelegate);
	DECLARE_MULTICAST_DELEGATE(FOnHealDelegate);
	
public:
	void BeginPlay() override;

	void SetHealth(float Health);
	float GetHealth() const;
	float GetMaxHealth() const;

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