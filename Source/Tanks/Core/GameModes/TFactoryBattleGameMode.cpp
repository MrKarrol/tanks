#include "TFactoryBattleGameMode.h"


#include "Tanks/Meta/TTankSpawner.h"
#include "Tanks/Components/THealthComponent.h"
#include "Tanks/Widgets/THUD.h"


void ATFactoryBattleGameMode::RegisterSpawner(ATTankSpawner* Spawner)
{
	if (Spawner)
	{
		Spawners.Add(Spawner);
		Spawner->HealthComponent->OnDieDelegate.AddUObject(this, &ATFactoryBattleGameMode::OnSpawnerDie);
	}

}

void ATFactoryBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (auto hud = Cast<ATHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		hud->ShowSideWidget(ESideWidgetType::SWT_PlayerState, 1);
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
