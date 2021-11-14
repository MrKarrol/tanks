#pragma once

#include "GameFramework/Actor.h"

#include "TProjectile.generated.h"


class UProjectileMovementComponent;
class UCapsuleComponent;

UCLASS()
class ATProjectile : public AActor
{
	GENERATED_BODY()
public:
	ATProjectile();

public:
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere)
		float Damage = 25.f;

protected:
	UFUNCTION()
		void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

};