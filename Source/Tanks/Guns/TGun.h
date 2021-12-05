#pragma once

#include "GameFramework/Actor.h"
#include "TGun.generated.h"

class UArrowComponent;
class UParticleSystemComponent;
class UAudioComponent;

UCLASS()
class ATGun : public AActor
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FOnGetScoreDelegate, float/*Score*/);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnShotDelegate, ATGun * /*Score*/);
	DECLARE_DELEGATE_OneParam(FOnChangeAmmoCapacityDelegate, int32 /*ammo*/);
public:
	ATGun();

	void StartFire();
	void StopFire();
	void AlternateFire();
	void Reload();
	bool CanFire() const;

public:
	int32 GetCurrentAmmo() const;

protected:
	virtual void DoFire();

public:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* TransformComponent;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere)
		UArrowComponent* FirePointComponent;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* FireFXComponent;

	UPROPERTY(VisibleAnywhere)
		UAudioComponent* FireAudioComponent;

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<UCameraShakeBase> ShotCameraShake;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	int Ammo = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	int AlternateAmmo = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	bool bInfiniteAmmo = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
	float Damage = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
	float FireSpeed = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
	bool NeedThirdView = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed", meta = (EditCondition = "NeedThirdView"))
	float TurretRotationSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed", meta = (EditCondition = "NeedThirdView"))
	float TurretRotationAcceleration = 64.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun")
	FString GunName;

	FOnGetScoreDelegate OnGetScoreDelegate;
	FOnShotDelegate OnShotDelegate;
	FOnChangeAmmoCapacityDelegate OnChangeAmmoCapacityDelegate;

protected:
	void OnGetScore(float Score);
	void ProceedDamage(AActor* damaged_actor);

protected:
	int32 m_current_ammo = Ammo;
	int mCurrentAlternateAmmo = AlternateAmmo;

	bool mIsFiring = false;

private:
	FTimerHandle mFireTimerHandle;

};