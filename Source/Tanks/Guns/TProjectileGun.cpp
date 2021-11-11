#include "TProjectileGun.h"

#include "Tanks/Projectiles/TProjectile.h"
#include "Components/ArrowComponent.h"
#include "Tanks/Tanks.h"


void ATProjectileGun::DoFire()
{
	if (DefaultProjectileClass)
	{
		if (CanFire())
		{
			Super::DoFire();

			FActorSpawnParameters spawn_params;
			spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			auto projectile = GetWorld()->SpawnActor<ATProjectile>(DefaultProjectileClass, FirePointComponent->GetComponentLocation(), FirePointComponent->GetComponentRotation(), spawn_params);
		}
	}
	else
	{
		UE_LOG(LogT, Error, TEXT("Projectile is null"));
	}
}