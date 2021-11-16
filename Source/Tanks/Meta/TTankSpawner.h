#pragma once

#include "GameFramework/Actor.h"
#include "TTankSpawner.generated.h"

class ATPatrolPoint;
class ATEnemyTank;
class UBoxComponent;
class UArrowComponent;

UCLASS()
class ATTankSpawner : public AActor
{
	GENERATED_BODY()
public:
	ATTankSpawner();
	void BeginPlay() override;

protected:
	void SpawnTank();

public:
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
		UArrowComponent* SpawnPointComponent;

	UPROPERTY(EditAnywhere)
		TArray<ATPatrolPoint*> PatrolPoints;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ATEnemyTank> DefaultTankClass;

	UPROPERTY(EditAnywhere)
		float SpawnRate = 2.f;

protected:
	FTimerHandle SpawnTimeHandler;

};