#include "TProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Tanks/Interfaces/IDamageTaker.h"


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
	if (auto damage_taker = Cast<IIDamageTaker>(ImpactResult.GetActor()))
	{
		FTDamageData data;
		data.Damage = Damage;
		data.Instigator = GetOwner();
		damage_taker->TakeDamage(data);
	}
	Destroy();
}
