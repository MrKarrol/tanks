#include "TProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"


ATProjectile::ATProjectile()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->bShouldBounce = true;
	MovementComponent->OnProjectileBounce.AddDynamic(this, &ATProjectile::OnBounce);
	
}

void ATProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	Destroy();
}
