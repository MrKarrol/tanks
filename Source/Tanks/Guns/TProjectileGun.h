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

protected:
	void DoFire() override;

};