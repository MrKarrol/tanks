#include "TTankSpawner.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Tanks/Meta/TPatrolPoint.h"
#include "Tanks/Enemies/TEnemyTank.h"

ATTankSpawner::ATTankSpawner()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(BoxComponent);

	SpawnPointComponent = CreateDefaultSubobject<UArrowComponent>("SpawnPointComponent");
	SpawnPointComponent->SetupAttachment(MeshComponent);
}

void ATTankSpawner::BeginPlay()
{
	Super::BeginPlay();

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
