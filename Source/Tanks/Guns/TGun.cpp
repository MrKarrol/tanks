#include "TGun.h"

#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Tanks/Interfaces/IDamageTaker.h"
#include "Tanks/Interfaces/IScorable.h"

ATGun::ATGun()
{
	TransformComponent = CreateDefaultSubobject<USceneComponent>("TransformComponent");
	SetRootComponent(TransformComponent);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>("GunMesh");
	GunMesh->SetupAttachment(TransformComponent);

	FirePointComponent = CreateDefaultSubobject<UArrowComponent>("FirePoint");
	FirePointComponent->SetupAttachment(GunMesh);

	FireFXComponent = CreateDefaultSubobject<UParticleSystemComponent>("FireFXComponent");
	FireFXComponent->SetupAttachment(FirePointComponent);

	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>("FireAudioComponent");
	FireAudioComponent->SetupAttachment(FirePointComponent);
}

void ATGun::StartFire()
{
	if ((m_current_ammo || bInfiniteAmmo) && !mIsFiring)
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
	m_current_ammo = Ammo;
	if (OnChangeAmmoCapacityDelegate.IsBound())
		OnChangeAmmoCapacityDelegate.Execute(m_current_ammo);
	mCurrentAlternateAmmo = AlternateAmmo;
}

bool ATGun::CanFire() const
{
	return m_current_ammo || bInfiniteAmmo;
}

int32 ATGun::GetCurrentAmmo() const
{
	return m_current_ammo;
}

void ATGun::DoFire()
{
	if (CanFire())
	{
		--m_current_ammo;
		if (OnChangeAmmoCapacityDelegate.IsBound())
			OnChangeAmmoCapacityDelegate.Execute(m_current_ammo);

		FireFXComponent->ActivateSystem();
		FireAudioComponent->Play();

		OnShotDelegate.Broadcast(this);
	}
}

void ATGun::OnGetScore(float Score)
{
	if (OnGetScoreDelegate.IsBound())
	{
		OnGetScoreDelegate.Execute(Score);
	}
}

void ATGun::ProceedDamage(AActor* damaged_actor)
{
	if (const auto damage_taker = Cast<IIDamageTaker>(damaged_actor))
	{
		FTDamageData data;
		data.Damage = Damage;
		data.Instigator = GetOwner();
		float possible_score{};
		if (const auto scorable = Cast<IIScorable>(damaged_actor))
			possible_score = scorable->GetScore();
		damage_taker->TakeDamage(data);
		if (damage_taker->IsDead())
		{
			if (OnGetScoreDelegate.IsBound())
			{
				OnGetScoreDelegate.Execute(possible_score);
			}
		}
	}
}
