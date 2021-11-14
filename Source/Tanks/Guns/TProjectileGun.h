#pragma once

#include "TGun.h"

#include "TProjectileGun.generated.h"

class ATProjectile;

UCLASS()
class ATProjectileGun : public ATGun
{
	GENERATED_BODY()
public:
	
public:
	UPROPERTY(EditAnywhere, Category = "Components")
		TSubclassOf <ATProjectile> DefaultProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Fire")
		float FireAccuracy = 2.f;

protected:
	void DoFire() override;

};