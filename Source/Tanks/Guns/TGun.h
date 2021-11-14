#pragma once

#include "GameFramework/Actor.h"

#include "TGun.generated.h"

class UArrowComponent;


UCLASS()
class ATGun : public AActor
{
	GENERATED_BODY()
public:
	ATGun();

	void StartFire();
	void StopFire();
	void AlternateFire();
	void Reload();
	bool CanFire() const;

protected:
	virtual void DoFire();

public:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* TransformComponent;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere)
		UArrowComponent* FirePointComponent;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		int Ammo = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		int AlternateAmmo = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		bool bInfiniteAmmo = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
		float FireSpeed = 1.f;

protected:
	int mCurrentAmmo = Ammo;
	int mCurrentAlternateAmmo = AlternateAmmo;

	bool mIsFiring = false;

private:
	FTimerHandle mFireTimerHandle;

};