#include "TEnemyTank.h"

#include "Tanks/Meta/TPatrolPoint.h"


ATEnemyTank::ATEnemyTank()
{
	bInfiniteAmmo = true;
}

void ATEnemyTank::OnDie()
{
	Super::OnDie();

	Destroy();
}
