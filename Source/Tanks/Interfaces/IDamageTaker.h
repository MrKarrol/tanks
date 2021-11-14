#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageTaker.generated.h"

USTRUCT()
struct FTDamageData
{
	GENERATED_BODY()

	float Damage = 0.f;

	UPROPERTY()
		AActor* Instigator = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDamageTaker : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TANKS_API IIDamageTaker
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void TakeDamage(const FTDamageData &) = 0;

};
