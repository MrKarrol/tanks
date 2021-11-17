#include "TFactoryBattleGameMode.h"

#include "Tanks/Meta/TTankSpawner.h"
#include "Tanks/Components/THealthComponent.h"


ATFactoryBattleGameMode::ATFactoryBattleGameMode()
{
}

void ATFactoryBattleGameMode::RegisterSpawner(ATTankSpawner* Spawner)
{
	if (Spawner)
	{
		Spawners.Add(Spawner);
		Spawner->HealthComponent->OnDieDelegate.AddUObject(this, &ATFactoryBattleGameMode::OnSpawnerDie);
	}

}

void ATFactoryBattleGameMode::OnSpawnerDie()
{
	bool bIsAllDead = true;
	for (auto spawner : Spawners)
		if (spawner && !spawner->IsDead())
		{
			bIsAllDead = false;
			break;
		}
	if (bIsAllDead)
		OnEndFactoryBattleDelegate.Broadcast();
}
