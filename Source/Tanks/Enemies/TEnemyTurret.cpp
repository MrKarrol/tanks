#include "TEnemyTurret.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"


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

AActor* ATEnemyTurret::GetTarget() const
{
	return UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

bool ATEnemyTurret::TargetIsVisible() const
{
	return GetTarget() != nullptr;
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