#include "TEnemyTankAIController.h"

#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Tanks/Enemies/TEnemyTank.h"


ATEnemyTankAIController::ATEnemyTankAIController()
{

}

void ATEnemyTankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TankPawn)
		return;

	if (TargetIsVisible())
	{
		ProcessTargeting(DeltaTime);
	}
	else
	{
		ProcessMovement(DeltaTime);
	}
	if (CanFire())
	{
		TankPawn->StartFire();
	}
	else
	{
		TankPawn->StopFire();
	}
}

AActor* ATEnemyTankAIController::GetTarget() const
{
	return UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

bool ATEnemyTankAIController::TargetIsVisible() const
{
	if (GetTarget() && TankPawn)
	{
		const FVector start_trace = TankPawn->GetActorLocation();
		const FVector end_trace = GetTarget()->GetActorLocation();
		FHitResult result;
		const TArray<AActor*> ignore_list{ TankPawn, GetTarget() };
		const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::Type::None;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), start_trace, end_trace, TraceChannel, false, ignore_list, DrawDebugType, result, true);
		
		return !result.bBlockingHit;
	}
	return false;
}

bool ATEnemyTankAIController::CanFire() const
{
	return GetTarget() && TargetIsVisible() && TankPawn->GetGunPivotAttach()->GetComponentRotation().Equals(GetRotationToTarget(), FireAccuracy);
}

void ATEnemyTankAIController::ProcessMovement(float DeltaTime)
{
	if (!TankPawn || TankPawn->PatrolPoints.Num() <= TargetPatrolPointIndex)
		return;
	if (FVector::Dist(TankPawn->GetActorLocation(), TankPawn->PatrolPoints[TargetPatrolPointIndex]->GetActorLocation()) <= AcceptanceRadius)
	{
		++TargetPatrolPointIndex;
		if (!TankPawn->PatrolPoints.IsValidIndex(TargetPatrolPointIndex))
			TargetPatrolPointIndex = 0;
	}

	const auto next_point_location = TankPawn->PatrolPoints[TargetPatrolPointIndex]->GetActorLocation();
	const auto rotation_to_point = UKismetMathLibrary::FindLookAtRotation(TankPawn->GetActorLocation(), next_point_location);
	TankPawn->AddTankMovementInput(1.f);
	TankPawn->AddTankRotationInput(rotation_to_point.Yaw);
}


void ATEnemyTankAIController::ProcessTargeting(float DeltaTime)
{
	if (TargetIsVisible())
	{
		TankPawn->AddTankTurretRotationInput(GetRotationToTarget());
	}
}

void ATEnemyTankAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	TankPawn = Cast<ATEnemyTank>(InPawn);
}

FRotator ATEnemyTankAIController::GetRotationToTarget() const
{
	if (GetTarget() && TankPawn)
	{
		FRotator new_turret_rotation = UKismetMathLibrary::FindLookAtRotation(TankPawn->GetGunPivotAttach()->GetComponentLocation(), GetTarget()->GetActorLocation());
		new_turret_rotation.Roll = TankPawn->GetGunPivotAttach()->GetComponentRotation().Roll;
		new_turret_rotation.Pitch = TankPawn->GetGunPivotAttach()->GetComponentRotation().Pitch;
		return new_turret_rotation;
	}
	return FRotator::ZeroRotator;
}
