#pragma once

#include "Tanks/Projectiles/TProjectile.h"
#include "TRadialProjectile.generated.h"

class UParticleSystemComponent;
class UAudioComponent;

UCLASS()
class ATRadialProjectile : public ATProjectile
{
	GENERATED_BODY()
public:
	ATRadialProjectile();

protected:
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity) override;

public:
	UPROPERTY(EditAnywhere)
		TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(EditAnywhere)
		float ExplodeRadius = 300.f;

	UPROPERTY(EditAnywhere)
		float PushForce = 100.f;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* DieFXComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UAudioComponent* DieAudioComponent;

};