#include "TPawn.h"

#include "Tanks/Components/THealthComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Tanks/Guns/TGun.h"
#include "Tanks/Tanks.h"

ATPawn::ATPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);

	BodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BodyMeshComponent");
	BodyMeshComponent->SetupAttachment(BoxComponent);

	HealthComponent = CreateDefaultSubobject<UTHealthComponent>("HealthComponent");
	HealthComponent->OnDieDelegate.AddUObject(this, &ATPawn::OnDie);
	HealthComponent->OnDamageDelegate.AddUObject(this, &ATPawn::OnDamage);

	DamageFXComponent = CreateDefaultSubobject<UParticleSystemComponent>("DamageFXComponent");
	DamageFXComponent->SetupAttachment(BodyMeshComponent);

	DamageAudioComponent = CreateDefaultSubobject<UAudioComponent>("DamageAudioComponent");
	DamageAudioComponent->SetupAttachment(BodyMeshComponent);

	DieFXComponent = CreateDefaultSubobject<UParticleSystemComponent>("DieFXComponent");
	DieFXComponent->SetupAttachment(BodyMeshComponent);

	DieAudioComponent = CreateDefaultSubobject<UAudioComponent>("DieAudioComponent");
	DieAudioComponent->SetupAttachment(BodyMeshComponent);
}

void ATPawn::BeginPlay()
{
	Super::BeginPlay();

	GunClassFirst = DefaultGunClassFirst;
	GunClassSecond = DefaultGunClassSecond;

	ChangeGun(GunClassFirst);
}

void ATPawn::Destroyed()
{
	Super::Destroyed();

	if (mGun)
		mGun->Destroy();
}

void ATPawn::TakeDamage(const FTDamageData& data)
{
	if (data.Instigator != this)
		HealthComponent->SetHealth(HealthComponent->GetHealth() - data.Damage);
}

bool ATPawn::IsDead() const
{
	return FMath::IsNearlyZero(HealthComponent->GetHealth());
}

float ATPawn::GetScore()
{
	return Score;
}

void ATPawn::StartFire()
{
	if (mGun)
	{
		mGun->StartFire();
	}
	else
	{
		UE_LOG(LogT, Error, TEXT("Gun is null"));
	}
}

void ATPawn::StopFire()
{
	if (mGun)
	{
		mGun->StopFire();
	}
	else
	{
		UE_LOG(LogT, Error, TEXT("Gun is null"));
	}
}

void ATPawn::AlternateFire()
{
	if (mGun)
	{
		mGun->AlternateFire();
	}
	else
	{
		UE_LOG(LogT, Error, TEXT("Gun is null"));
	}
}

void ATPawn::Reload()
{
	if (mGun)
	{
		mGun->Reload();
	}
	else
	{
		UE_LOG(LogT, Error, TEXT("Gun is null"));
	}
}

void ATPawn::SwapGuns()
{
	switch (mCurrentGun)
	{
	case 0:
		if (GunClassSecond)
		{
			ChangeGun(GunClassSecond);
			mCurrentGun = 1;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("You now powered with second gun"));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("You haven't second gun"));
		break;
	case 1:
		if (GunClassSecond)
		{
			ChangeGun(GunClassFirst);
			mCurrentGun = 0;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("You now powered with first gun"));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("You haven't first gun"));
		break;
	}
}

void ATPawn::SetGun(TSubclassOf<ATGun> GunClass)
{
	if (GunClass == GunClassFirst || GunClass == GunClassSecond)
		return;

	switch (mCurrentGun)
	{
	case 0:
		if (!GunClassSecond)
		{
			GunClassSecond = GunClass;
			SwapGuns();
			return;
		}
		GunClassFirst = GunClass;
		ChangeGun(GunClassFirst);
		break;
	case 1:
		if (!GunClassFirst)
		{
			GunClassFirst = GunClass;
			SwapGuns();
			return;
		}
		GunClassSecond = GunClass;
		ChangeGun(GunClassSecond);
		break;
	}
}

void ATPawn::ChangeGun(TSubclassOf<ATGun> GunClass)
{
	if (GunClass)
	{
		if (mGun)
			mGun->Destroy();

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		mGun = GetWorld()->SpawnActor<ATGun>(GunClass, GetGunPivotAttach()->GetComponentLocation(), GetGunPivotAttach()->GetComponentRotation(), spawnParams);
		mGun->bInfiniteAmmo = bInfiniteAmmo;
		mGun->AttachToComponent(GetGunPivotAttach(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Gun");
		mGun->SetOwner(this);
	}
}

USceneComponent* ATPawn::GetGunPivotAttach() const
{
	return BodyMeshComponent;
}

void ATPawn::OnDie()
{
	DieFXComponent->ActivateSystem();
	DieAudioComponent->Play();
}

void ATPawn::OnDamage()
{
	DamageFXComponent->ActivateSystem();
	GetWorld()->GetTimerManager().SetTimer(mDamageTimerHandle, DamageFXComponent, &UParticleSystemComponent::DeactivateSystem, 2.f, false, 0.f);
	DamageAudioComponent->Play();
}
