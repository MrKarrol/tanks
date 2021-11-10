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
	if (mCurrentAmmo && !mIsFiring)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Start fire")));
		GetWorld()->GetTimerManager().SetTimer(mFireTimerHandle, this, &ATGun::DoFire, FireSpeed, true, 0.f);
		mIsFiring = true;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Need reload")));
}

void ATGun::StopFire()
{
	if (mIsFiring)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Stop fire")));
		GetWorld()->GetTimerManager().ClearTimer(mFireTimerHandle);
		mIsFiring = false;
	}
}

void ATGun::AlternateFire()
{
	if (mCurrentAlternateAmmo)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Dynamite")));
		--mCurrentAlternateAmmo;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Need ammo")));
}

void ATGun::Reload()
{
	mCurrentAmmo = Ammo;
	mCurrentAlternateAmmo = AlternateAmmo;
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Reloaded")));
}

void ATGun::DoFire()
{
	if (mCurrentAmmo)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Fire")));
		--mCurrentAmmo;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Need ammo")));
}