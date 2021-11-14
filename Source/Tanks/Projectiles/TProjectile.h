#pragma once

#include "GameFramework/Actor.h"

#include "TProjectile.generated.h"


class UProjectileMovementComponent;
class UCapsuleComponent;

UCLASS()
class ATProjectile : public AActor
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FOnHitSomethingDelegate, AActor * /*Target*/);
public:
	ATProjectile();

public:
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* MovementComponent;

	FOnHitSomethingDelegate OnHitSomethingDelegate;

protected:
	UFUNCTION()
		void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

};