#include "TGun.h"


ATGun::ATGun()
{
	TransformComponent = CreateDefaultSubobject<USceneComponent>("TransformComponent");
	SetRootComponent(TransformComponent);
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>("GunMesh");
	GunMesh->SetupAttachment(TransformComponent);
}

void ATGun::Fire()
{
	if (mCurrentAmmo)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Fire")));
		--mCurrentAmmo;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Need reload")));

}

void ATGun::AlternateFire()
{
	if (mCurrentAlternateAmmo)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Dynamite")));
		--mCurrentAlternateAmmo;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Need reload")));
}

void ATGun::Reload()
{
	mCurrentAmmo = Ammo;
	mCurrentAlternateAmmo = AlternateAmmo;
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Reloaded")));
}