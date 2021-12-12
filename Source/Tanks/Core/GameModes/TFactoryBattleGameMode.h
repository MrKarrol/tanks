#pragma once

#include "TGameMode.h"
#include "TFactoryBattleGameMode.generated.h"

class ATTankSpawner;
class ATPawn;


UCLASS()
class TANKS_API ATFactoryBattleGameMode : public ATGameMode
{
	GENERATED_BODY()
	DECLARE_MULTICAST_DELEGATE(FOnEndFactoryBattleDelegate);
	
public:
	void RegisterSpawner(ATTankSpawner* Spawner);

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	void OnSpawnerDie();

private:
	void OnPlayerDie() const;

public:
	FOnEndFactoryBattleDelegate OnEndFactoryBattleDelegate;
	
protected:
	UPROPERTY()
	TArray< ATTankSpawner*> Spawners;

};
