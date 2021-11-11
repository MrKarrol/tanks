#include "TGun.h"

#include "Components/ArrowComponent.h"


ATGun::ATGun()
{
	TransformComponent = CreateDefaultSubobject<USceneComponent>("TransformComponent");
	SetRootComponent(TransformComponent);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>("GunMesh");
	GunMesh->SetupAttachment(TransformComponent);

	FirePointComponent = CreateDefaultSubobject<UArrowComponent>("FirePoint");
	FirePointComponent->SetupAttachment(GunMesh);
}

void ATGun::StartFire()
{
	if ((mCurrentAmmo || bInfiniteAmmo) && !mIsFiring)
	{
		GetWorld()->GetTimerManager().SetTimer(mFireTimerHandle, this, &ATGun::DoFire, FireSpeed, true, 0.f);
		mIsFiring = true;
	}
}

void ATGun::StopFire()
{
	if (mIsFiring)
	{
		GetWorld()->GetTimerManager().ClearTimer(mFireTimerHandle);
		mIsFiring = false;
	}
}

void ATGun::AlternateFire()
{
}

void ATGun::Reload()
{
	mCurrentAmmo = Ammo;
	mCurrentAlternateAmmo = AlternateAmmo;
}

bool ATGun::CanFire() const
{
	return mCurrentAmmo || bInfiniteAmmo;
}

void ATGun::DoFire()
{
	--mCurrentAmmo;
}