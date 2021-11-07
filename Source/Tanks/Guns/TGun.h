#pragma once

#include "GameFramework/Actor.h"

#include "TGun.generated.h"


UCLASS()
class ATGun : public AActor
{
	GENERATED_BODY()
public:
	ATGun();

	void Fire();
	void AlternateFire();
	void Reload();

public:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* TransformComponent;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* GunMesh;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		int Ammo = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		int AlternateAmmo = 5;

private:
	int mCurrentAmmo = Ammo;
	int mCurrentAlternateAmmo = AlternateAmmo;

};