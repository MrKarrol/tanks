#include "TProjectileGun.h"

#include "Tanks/Projectiles/TProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Tanks/Tanks.h"



ATProjectileGun::ATProjectileGun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATProjectileGun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bUsePredictProjectilePathParams && PredictProjectilePathParams.DrawDebugType != EDrawDebugTrace::None && DefaultProjectileClass)
	{
		PredictProjectilePathParams.StartLocation = FirePointComponent->GetComponentLocation();
		auto projectile_default = DefaultProjectileClass.GetDefaultObject();
		const auto initial_speed = projectile_default->MovementComponent->InitialSpeed;
		PredictProjectilePathParams.LaunchVelocity = FirePointComponent->GetForwardVector() * initial_speed;
		PredictProjectilePathParams.OverrideGravityZ = GetWorld()->GetWorldSettings()->GetGravityZ() * projectile_default->MovementComponent->ProjectileGravityScale;
		PredictProjectilePathParams.ProjectileRadius = projectile_default->CapsuleComponent->GetScaledCapsuleRadius();

		FPredictProjectilePathResult predictResult;
		UGameplayStatics::PredictProjectilePath(this, PredictProjectilePathParams, predictResult);
	}
}

void ATProjectileGun::DoFire()
{
	Super::DoFire();

	if (DefaultProjectileClass)
	{
		if (CanFire())
		{
			FActorSpawnParameters spawn_params;
			spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			auto projectile_rotation = FirePointComponent->GetComponentRotation();
			projectile_rotation.Yaw += FMath::RandRange(-FireAccuracy, FireAccuracy);
			auto projectile = GetWorld()->SpawnActor<ATProjectile>(DefaultProjectileClass, FirePointComponent->GetComponentLocation(), projectile_rotation, spawn_params);
			projectile->OnHitSomethingDelegate.BindUObject(this, &ATProjectileGun::ProceedDamage);
		}
	}
	else
	{
		UE_LOG(LogT, Error, TEXT("Projectile is null"));
	}
}
