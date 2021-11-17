#pragma once

#include "TGameMode.h"
#include "TFactoryBattleGameMode.generated.h"

class ATTankSpawner;


UCLASS()
class TANKS_API ATFactoryBattleGameMode : public ATGameMode
{
	GENERATED_BODY()
	DECLARE_MULTICAST_DELEGATE(FOnEndFactoryBattleDelegate);
public:
	ATFactoryBattleGameMode();

public:
	void RegisterSpawner(ATTankSpawner* Spawner);

protected:
	void OnSpawnerDie();

public:
	FOnEndFactoryBattleDelegate OnEndFactoryBattleDelegate;

protected:
	UPROPERTY()
	TArray< ATTankSpawner*> Spawners;

};
