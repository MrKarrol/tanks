#include "TPawn.h"

#include "Tanks/Components/THealthComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Tanks/Guns/TGun.h"

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
	DieAudioComponent->bStopWhenOwnerDestroyed = false;
}

void ATPawn::BeginPlay()
{
	Super::BeginPlay();

	for (const auto gun : PossessedGunsClasses)
		CreateGun(gun.Key, gun.Value);
	if (m_possessed_guns.Contains(EGunSlot::GS_First))
	{
		m_current_gun = EGunSlot::GS_First;
	}
	if (auto gun = GetCurrentGun())
		gun->SetHidden(false);
}

void ATPawn::Destroyed()
{
	Super::Destroyed();

	for (const auto gun : m_possessed_guns)
		gun.Value->Destroy();
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

UTHealthComponent* ATPawn::GetHealthComponent() const
{
	return HealthComponent;
}

void ATPawn::StartFire()
{
	if (auto gun = GetCurrentGun())
	{
		gun->StartFire();
	}
}

void ATPawn::StopFire()
{
	if (auto gun = GetCurrentGun())
	{
		gun->StopFire();
	}
}

void ATPawn::AlternateFire()
{
	if (auto gun = GetCurrentGun())
	{
		gun->AlternateFire();
	}
}

void ATPawn::Reload()
{
	for (const auto gun : m_possessed_guns)
		gun.Value->Reload();
}

void ATPawn::SwapGuns()
{
	const auto previous_gun = m_current_gun;
	switch (m_current_gun)
	{
	case EGunSlot::GS_First:
		if (m_possessed_guns.Contains(EGunSlot::GS_Second))
		{
			m_current_gun = EGunSlot::GS_Second;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("You now powered with second gun"));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("You haven't second gun"));
		break;
	case EGunSlot::GS_Second:
		if (m_possessed_guns.Contains(EGunSlot::GS_First))
		{
			m_current_gun = EGunSlot::GS_First;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("You now powered with first gun"));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("You haven't first gun"));
		break;
	default: break;
	}
	if (auto gun = m_possessed_guns.Find(previous_gun))
	{
		(*gun)->SetActorHiddenInGame(true);
		(*gun)->SetActorEnableCollision(false);
	}
		
	if (auto gun = GetCurrentGun())
	{
		gun->SetActorHiddenInGame(false);
		gun->SetActorEnableCollision(true);
	}
}

ATGun* ATPawn::GetCurrentGun() const
{
	if (m_possessed_guns.Contains(m_current_gun))
		return m_possessed_guns[m_current_gun];
	else
		return nullptr;
}

const TMap<EGunSlot, ATGun*>& ATPawn::GetGuns() const
{
	return m_possessed_guns;
}

void ATPawn::SetGun(TSubclassOf<ATGun> gun_class)
{
	for (const auto gun : PossessedGunsClasses)
		if (gun.Value == gun_class)
			return;

	switch (m_current_gun)
	{
	case EGunSlot::GS_First:
		AddOrReplaceGun(EGunSlot::GS_Second, EGunSlot::GS_First, gun_class);
		return;
	case EGunSlot::GS_Second:
	case EGunSlot::GS_NoSlot:
		AddOrReplaceGun(EGunSlot::GS_First, EGunSlot::GS_Second, gun_class);
		return;
	default: break;
	}
}


void ATPawn::CreateGun(const EGunSlot slot, const TSubclassOf<ATGun> gun_class)
{
	if (gun_class && slot != EGunSlot::GS_NoSlot)
	{
		if (const auto gun = m_possessed_guns.Find(slot))
		{
			(*gun)->Destroy();
		}

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_possessed_guns.Add(slot, GetWorld()->SpawnActor<ATGun>(gun_class, GetGunPivotAttach()->GetComponentLocation(), GetGunPivotAttach()->GetComponentRotation(), spawnParams));
		if (const auto gun = m_possessed_guns[slot])
		{
			gun->bInfiniteAmmo = bInfiniteAmmo;
			gun->AttachToComponent(GetGunPivotAttach(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Gun");
			gun->SetOwner(this);
			OnGunChangeDelegate.Execute(slot, gun); // show gun name in player state widget?
		}
	}
}

void ATPawn::AddOrReplaceGun(const EGunSlot slot_to_check, const EGunSlot slot_to_replace_if_slot_to_check_exist, const TSubclassOf<ATGun> gun_class)
{
	if (!PossessedGunsClasses.Contains(slot_to_check))
	{
		PossessedGunsClasses.Add(slot_to_check, gun_class);
		CreateGun(slot_to_check, gun_class);
		SwapGuns();
	}
	else
	{
		PossessedGunsClasses[slot_to_replace_if_slot_to_check_exist] = gun_class;
		CreateGun(slot_to_replace_if_slot_to_check_exist, gun_class);
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
	GetWorld()->GetTimerManager().SetTimer(m_damage_timer_handle, DamageFXComponent, &UParticleSystemComponent::DeactivateSystem, 1.f, false, 1.f);
	DamageAudioComponent->Play();
}
