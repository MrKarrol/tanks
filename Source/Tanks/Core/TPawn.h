#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TPawn.generated.h"

class ATGun;
class UArrowComponent;
class UBoxComponent;
class UTHealthComponent;

UCLASS()
class TANKS_API ATPawn : public APawn
{
	GENERATED_BODY()

public:
	ATPawn();
	
	void Destroyed() override;

	void StartFire();
	void StopFire();

	void AlternateFire();

	void Reload();

	void SwapGuns();

	void SetGun(TSubclassOf<ATGun> GunClass);

	virtual USceneComponent* GetGunPivotAttach() const;

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
		TSubclassOf<ATGun> DefaultGunClassFirst;

	UPROPERTY(EditAnywhere, Category = "Components")
		TSubclassOf<ATGun> DefaultGunClassSecond;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BoxComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		bool bInfiniteAmmo = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UTHealthComponent* HealthComponent;

protected:
	virtual void BeginPlay() override;

	virtual void ChangeGun(TSubclassOf<ATGun> GunClass);
	
	virtual void OnDie();

protected:
	UPROPERTY()
		ATGun* mGun = nullptr;

	UPROPERTY()
		TSubclassOf<ATGun> GunClassFirst;

	UPROPERTY()
		TSubclassOf<ATGun> GunClassSecond;

	int mCurrentGun = 0;

};
