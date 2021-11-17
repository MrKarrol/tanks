#include "TTankSpawner.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Tanks/Components/THealthComponent.h"
#include "Tanks/Core/GameModes/TFactoryBattleGameMode.h"
#include "Tanks/Meta/TPatrolPoint.h"
#include "Tanks/Enemies/TEnemyTank.h"

ATTankSpawner::ATTankSpawner()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(BoxComponent);

	SpawnPointComponent = CreateDefaultSubobject<UArrowComponent>("SpawnPointComponent");
	SpawnPointComponent->SetupAttachment(BoxComponent);

	HealthComponent = CreateDefaultSubobject<UTHealthComponent>("HealthComponent");
	HealthComponent->OnDieDelegate.AddUObject(this, &ATTankSpawner::OnDie);
	HealthComponent->OnDamageDelegate.AddUObject(this, &ATTankSpawner::OnDamage);
}

void ATTankSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (auto game_mode = Cast<ATFactoryBattleGameMode>(GetWorld()->GetAuthGameMode()))
	{
		game_mode->RegisterSpawner(this);
	}

	FTimerHandle SpawnTimeHandler;
	GetWorldTimerManager().SetTimer(SpawnTimeHandler, this, &ATTankSpawner::SpawnTank, SpawnRate, true);
}

void ATTankSpawner::SpawnTank()
{
	if (!DefaultTankClass)
		return;

	auto tank = GetWorld()->SpawnActorDeferred<ATEnemyTank>(DefaultTankClass, 
		SpawnPointComponent->GetComponentTransform(), 
		this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	tank->PatrolPoints = PatrolPoints;
	tank->FinishSpawning(SpawnPointComponent->GetComponentTransform());
}

void ATTankSpawner::TakeDamage(const FTDamageData& data)
{
	if (data.Instigator != this)
		HealthComponent->SetHealth(HealthComponent->GetHealth() - data.Damage);
}

bool ATTankSpawner::IsDead() const
{
	return FMath::IsNearlyZero(HealthComponent->GetHealth());
}

float ATTankSpawner::GetScore()
{
	return Score;
}

void ATTankSpawner::OnDie()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	Destroy();
}

void ATTankSpawner::OnDamage()
{
}