#pragma once

#include "CoreMinimal.h"
#include "Tanks/Core/TTankPawn.h"
#include "Tanks/Meta/TPatrolPoint.h"

#include "TEnemyTank.generated.h"


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