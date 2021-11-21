#include "TProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"


ATProjectile::ATProjectile()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	SetRootComponent(CapsuleComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(CapsuleComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->bShouldBounce = true;
	MovementComponent->OnProjectileBounce.AddDynamic(this, &ATProjectile::OnBounce);
}


void ATProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (ImpactResult.GetActor() == GetOwner())
		return;

	if (OnHitSomethingDelegate.IsBound())
		OnHitSomethingDelegate.Execute(ImpactResult.GetActor());
	Destroy();
}
