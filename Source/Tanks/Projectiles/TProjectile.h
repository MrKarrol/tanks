#pragma once

#include "GameFramework/Actor.h"

#include "TProjectile.generated.h"


class UProjectileMovementComponent;

UCLASS()
class ATProjectile : public AActor
{
	GENERATED_BODY()
public:
	ATProjectile();

public:
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* MovementComponent;

protected:
	UFUNCTION()
		void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

};