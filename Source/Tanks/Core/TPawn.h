#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tanks/Interfaces/IDamageTaker.h"
#include "Tanks/Interfaces/IScorable.h"
#include "TPawn.generated.h"

class ATGun;
class UArrowComponent;
class UBoxComponent;
class UTHealthComponent;
class UParticleSystemComponent;
class UAudioComponent;


UENUM()
enum class EGunSlot : uint8
{
	GS_NoSlot = 0,
	GS_First,
	GS_Second
};

UCLASS()
class TANKS_API ATPawn : public APawn, public IIScorable, public IIDamageTaker
{
	GENERATED_BODY()

	DECLARE_DELEGATE_TwoParams(FOnGunChangeDelegate, const EGunSlot, ATGun *);

public:
	ATPawn();
	
	virtual void Destroyed() override;

	void StartFire();
	void StopFire();
	void AlternateFire();
	void Reload();
	
	
	const TMap<EGunSlot, ATGun *> &GetGuns() const;

	virtual void SetGun(TSubclassOf<ATGun> gun_class);

	virtual USceneComponent* GetGunPivotAttach() const;

	//= Begin IDamageTaker interface
	virtual void TakeDamage(const FTDamageData&) override;
	virtual bool IsDead() const override;
	//= End IDamageTaker interface

	//= Begin IScorable interface
	virtual float GetScore() override;
	//= End IScorable interface

	UTHealthComponent * GetHealthComponent() const;

	FOnGunChangeDelegate OnGunChangeDelegate;

protected:
	ATGun * GetCurrentGun() const;

	virtual void BeginPlay() override;
	virtual void SwapGuns();

	virtual void OnDie();
	virtual void OnDamage();

private:
	void CreateGun(const EGunSlot slot, const TSubclassOf<ATGun> gun_class);
	void AddOrReplaceGun(const EGunSlot slot_to_check, const EGunSlot slot_to_replace_if_slot_to_check_exist, const TSubclassOf<ATGun> gun_class);

public:
	UPROPERTY(EditAnywhere, Category = "Score")
	float Score = 200.f;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	TMap<EGunSlot,TSubclassOf<ATGun>> PossessedGunsClasses;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BoxComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		bool bInfiniteAmmo = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UTHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* DamageFXComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UAudioComponent* DamageAudioComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* DieFXComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UAudioComponent* DieAudioComponent;

private:
	TMap<EGunSlot, ATGun *> m_possessed_guns;
	EGunSlot m_current_gun;
	FTimerHandle m_damage_timer_handle;

};
