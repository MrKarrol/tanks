#pragma once

#include "AIModule/Classes/AIController.h"
#include "TEnemyTankAIController.generated.h"

class ATEnemyTank;


UCLASS()
class ATEnemyTankAIController : public AAIController
{
	GENERATED_BODY()
public:
	ATEnemyTankAIController();

	void Tick(float DeltaTime) override;

	AActor* GetTarget() const;
	bool TargetIsVisible() const;
	bool CanFire() const;

protected:
	void ProcessMovement(float DeltaTime);
	void ProcessTargeting(float DeltaTime);
	void OnPossess(APawn*) override;
	FRotator GetRotationToTarget() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
		TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
		float FireAccuracy = 1.0;

	UPROPERTY()
		ATEnemyTank* TankPawn;

	int32 TargetPatrolPointIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float AcceptanceRadius = 50.f;

};