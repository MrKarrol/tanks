#include "TEnemyTank.h"


ATEnemyTank::ATEnemyTank()
{
	bInfiniteAmmo = true;
}

void ATEnemyTank::OnDie()
{
	Super::OnDie();

	Destroy();
}
