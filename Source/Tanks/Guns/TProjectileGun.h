#pragma once

#include "TGun.h"
#include "Kismet/GameplayStatics.h"
#include "TProjectileGun.generated.h"

class ATProjectile;

UCLASS()
class ATProjectileGun : public ATGun
{
	GENERATED_BODY()
public:
	ATProjectileGun();
	void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "Components")
		TSubclassOf <ATProjectile> DefaultProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Fire")
		float FireAccuracy = 2.f;

	UPROPERTY(EditAnywhere)
		bool bUsePredictProjectilePathParams = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUsePredictProjectilePathParams"))
		FPredictProjectilePathParams	PredictProjectilePathParams;


protected:
	void DoFire() override;

};