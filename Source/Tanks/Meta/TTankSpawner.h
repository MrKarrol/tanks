#pragma once

#include "GameFramework/Actor.h"
#include "Tanks/Interfaces/IDamageTaker.h"
#include "Tanks/Interfaces/IScorable.h"
#include "TTankSpawner.generated.h"

class ATPatrolPoint;
class ATEnemyTank;
class UBoxComponent;
class UArrowComponent;
class UTHealthComponent;

UCLASS()
class ATTankSpawner : public AActor, public IIScorable, public IIDamageTaker
{
	GENERATED_BODY()
public:
	ATTankSpawner();
	void BeginPlay() override;

	//= Begin IDamageTaker interface
	void TakeDamage(const FTDamageData&) override;
	bool IsDead() const override;
	//= End IDamageTaker interface

	//= Begin IScorable interface
	float GetScore() override;
	//= End IScorable interface

protected:
	void SpawnTank();

	virtual void OnDie();
	virtual void OnDamage();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* SpawnPointComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UTHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USkeletalMesh* MeshAfterDestroying;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UParticleSystemComponent* SpawnFXComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UAudioComponent* SpawnAudioComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UParticleSystemComponent* DieFXComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UAudioComponent* DieAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		TArray<ATPatrolPoint*> PatrolPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn")
		TSubclassOf<ATEnemyTank> DefaultTankClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn")
		float SpawnRate = 2.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Score")
		float Score = 200.f;

protected:
	FTimerHandle mSpawnTimerHandle;
	float bScoreTaken = false;

};