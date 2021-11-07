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
		int Ammo = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		int AlternateAmmo = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
		float FireSpeed = 1.f;

private:
	FTimerHandle mFireTimerHandle;

private:
	int mCurrentAmmo = Ammo;
	int mCurrentAlternateAmmo = AlternateAmmo;

};