#include "TFactoryBattleGameMode.h"


#include "Tanks/Meta/TTankSpawner.h"
#include "Tanks/Components/THealthComponent.h"
#include "Tanks/Player/TPlayerPawn.h"
#include "Tanks/Widgets/THUD.h"


void ATFactoryBattleGameMode::RegisterSpawner(ATTankSpawner* Spawner)
{
	if (Spawner)
	{
		Spawners.Add(Spawner);
		Spawner->HealthComponent->OnDieDelegate.AddUObject(this, &ATFactoryBattleGameMode::OnSpawnerDie);
	}

	if (const auto player = Cast<ATPlayerPawn>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		player->GetHealthComponent()->OnDieDelegate.AddUObject(this, &ATFactoryBattleGameMode::OnPlayerDie);
	}
}

void ATFactoryBattleGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

void ATFactoryBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (auto hud = Cast<ATHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		hud->ShowSideWidget(ESideWidgetType::SWT_PlayerState, 1);
		hud->ShowSideWidget(ESideWidgetType::SWT_MiniMap, 1);
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

void ATFactoryBattleGameMode::OnPlayerDie() const
{
	if (auto hud = Cast<ATHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		hud->HideSideWidget(ESideWidgetType::SWT_MiniMap);
		hud->ShowMainWidget(EMainWidgetType::MWT_GameOverMenu);
	}
}
