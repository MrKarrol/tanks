#include "TTankSpawner.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Tanks/Components/THealthComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
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

	SpawnFXComponent = CreateDefaultSubobject<UParticleSystemComponent>("SpawnFXComponent");
	SpawnFXComponent->SetupAttachment(SpawnPointComponent);

	SpawnAudioComponent = CreateDefaultSubobject<UAudioComponent>("SpawnAudioComponent");
	SpawnAudioComponent->SetupAttachment(SpawnPointComponent);

	DieFXComponent = CreateDefaultSubobject<UParticleSystemComponent>("DieFXComponent");
	DieFXComponent->SetupAttachment(MeshComponent);

	DieAudioComponent = CreateDefaultSubobject<UAudioComponent>("DieAudioComponent");
	DieAudioComponent->SetupAttachment(MeshComponent);
	DieAudioComponent->bStopWhenOwnerDestroyed = false; // it will play sound if MeshAfterDestroying was not set
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

	SpawnFXComponent->ActivateSystem();
	SpawnAudioComponent->Play();
	GetWorldTimerManager().SetTimer(mSpawnTimerHandle, SpawnFXComponent,
		&UParticleSystemComponent::DeactivateSystem, 1.f, false, 1.f);

	auto tank = GetWorld()->SpawnActorDeferred<ATEnemyTank>(DefaultTankClass, 
		SpawnPointComponent->GetComponentTransform(), 
		this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	tank->PatrolPoints = PatrolPoints;
	tank->FinishSpawning(SpawnPointComponent->GetComponentTransform());
}

void ATTankSpawner::TakeDamage(const FTDamageData& data)
{
	if (IsDead())
		return;
	if (data.Instigator != this)
		HealthComponent->SetHealth(HealthComponent->GetHealth() - data.Damage);
}

bool ATTankSpawner::IsDead() const
{
	return FMath::IsNearlyZero(HealthComponent->GetHealth());
}

float ATTankSpawner::GetScore()
{
	if (!bScoreTaken)
	{
		bScoreTaken = true;
		return Score;
	}
	return 0.f;
}

void ATTankSpawner::OnDie()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);

	DieFXComponent->ActivateSystem();
	DieAudioComponent->Play();

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Dead")));
	if (MeshAfterDestroying)
		MeshComponent->SetSkeletalMesh(MeshAfterDestroying);
	else
		Destroy();
}

void ATTankSpawner::OnDamage()
{
}