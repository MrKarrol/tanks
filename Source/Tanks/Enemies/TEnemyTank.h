#pragma once

#include "CoreMinimal.h"
#include "Tanks/Core/TTankPawn.h"
#include "TEnemyTank.generated.h"

class ATPatrolPoint;

UCLASS()
class TANKS_API ATEnemyTank : public ATTankPawn
{
	GENERATED_BODY()

public:
	ATEnemyTank();

protected:
	void OnDie() override;

public:
	UPROPERTY(EditAnywhere)
		TArray<ATPatrolPoint*> PatrolPoints;

};