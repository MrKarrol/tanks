#include "TEnemyTurret.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Tanks/Components/THealthComponent.h"
#include "Tanks/Guns/TGun.h"


ATEnemyTurret::ATEnemyTurret()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GunPivotLocation = CreateDefaultSubobject<UArrowComponent>("GunPivotLocation");
	GunPivotLocation->SetupAttachment(BodyMeshComponent);

	bInfiniteAmmo = true;
}

void ATEnemyTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProcessTargeting(DeltaTime);
}

void ATEnemyTurret::SetInitialGun(TSubclassOf<ATGun> InitialGun)
{
	PossessedGunsClasses.Add(EGunSlot::GS_First, InitialGun);
}

AActor* ATEnemyTurret::GetTarget() const
{
	return UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

bool ATEnemyTurret::TargetIsVisible() const
{
	auto gun = GetCurrentGun();
	if (gun && GetTarget())
	{
		const FVector start_trace = gun->GetActorLocation();
		const FVector end_trace = GetTarget()->GetActorLocation();
		FHitResult result;

		const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::Type::None;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), start_trace, end_trace, TraceChannel, false, {}, DrawDebugType, result, true);

		return !result.bBlockingHit;
	}
	return false;
}

void ATEnemyTurret::ProcessTargeting(float DeltaTime)
{
	if (GetTarget() && TargetIsVisible())
	{
		FRotator new_turret_direction = UKismetMathLibrary::FindLookAtRotation(GetGunPivotAttach()->GetComponentLocation(), GetTarget()->GetActorLocation());
		new_turret_direction.Roll = GetGunPivotAttach()->GetComponentRotation().Roll;
		new_turret_direction.Pitch = GetGunPivotAttach()->GetComponentRotation().Pitch;

		GetGunPivotAttach()->SetWorldRotation(new_turret_direction);
		StartFire();
	}
	else
		StopFire();
}

USceneComponent* ATEnemyTurret::GetGunPivotAttach() const
{
	return GunPivotLocation;
}

void ATEnemyTurret::OnDie()
{
	Super::OnDie();

	Destroy();
}
