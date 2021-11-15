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

UCLASS()
class TANKS_API ATPawn : public APawn, public IIScorable, public IIDamageTaker
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

	virtual void SetGun(TSubclassOf<ATGun> GunClass);

	virtual USceneComponent* GetGunPivotAttach() const;

	//= Begin IDamageTaker interface
	void TakeDamage(const FTDamageData&) override;
	bool IsDead() const override;
	//= End IDamageTaker interface

	//= Begin IScorable interface
	float GetScore() override;
	//= End IScorable interface

public:
	UPROPERTY(EditAnywhere, Category = "Score")
		float Score = 200.f;

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* DamageFXComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UAudioComponent* DamageAudioComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* DieFXComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UAudioComponent* DieAudioComponent;

protected:
	virtual void BeginPlay() override;

	virtual void ChangeGun(TSubclassOf<ATGun> GunClass);
	
	virtual void OnDie();
	virtual void OnDamage();

protected:
	UPROPERTY()
		ATGun* mGun = nullptr;

	UPROPERTY()
		TSubclassOf<ATGun> GunClassFirst;

	UPROPERTY()
		TSubclassOf<ATGun> GunClassSecond;

	int mCurrentGun = 0;

private:
	FTimerHandle mDamageTimerHandle;

};
