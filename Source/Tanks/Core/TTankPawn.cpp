#include "TTankPawn.h"

#include "Components/ArrowComponent.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Tanks/Tanks.h"
#include "Tanks/Guns/TGun.h"


ATTankPawn::ATTankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	GunPivotLocation = CreateDefaultSubobject<UArrowComponent>("GunPivotLocation");
	GunPivotLocation->SetupAttachment(BodyMeshComponent);
}
	
void ATTankPawn::PerformMovement(float DeltaTime)
{
	const auto move_sign = FMath::Sign(mCachedMoveDelta);
	auto rotation_sign = FMath::Sign(mCachedRotationDelta);
	if (!FMath::IsNearlyZero(mCachedMoveDelta))
		rotation_sign *= move_sign;
	// move
	if (FMath::IsNearlyZero(mCachedMoveDelta))
		mCurrentMoveSpeed = 0.f;
	else
	{
		mCurrentMoveSpeed = FMath::FInterpConstantTo(mCurrentMoveSpeed, MoveSpeed * FMath::Abs(mCachedMoveDelta), DeltaTime, MoveAcceleration);
		const FVector position_delta = GetActorForwardVector() * mCurrentMoveSpeed * DeltaTime * move_sign;
		SetActorLocation(GetActorLocation() + position_delta);

		mCachedMoveDelta = 0.f;
	}

	// rotation
	if (FMath::IsNearlyZero(mCachedRotationDelta))
		mCurrentRotationSpeed = 0.f;
	else
	{
		mCurrentRotationSpeed = FMath::FInterpConstantTo(mCurrentRotationSpeed, RotationSpeed * FMath::Abs(mCachedRotationDelta), DeltaTime, RotationAcceleration);
		const FRotator rotation_delta = FRotator(0.f, mCurrentRotationSpeed * DeltaTime * rotation_sign, 0.f);
		SetActorRotation(GetActorRotation() + rotation_delta);

		mCachedRotationDelta = 0.f;
	}
}

void ATTankPawn::PerformTurretRotation()
{
	GunPivotLocation->SetWorldRotation(mCachedTurretRotationDelta);
}

USceneComponent* ATTankPawn::GetGunPivotAttach() const
{
	return GunPivotLocation;
}

void ATTankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformMovement(DeltaTime);
	PerformTurretRotation();
}

void ATTankPawn::AddTankMovementInput(float MoveDelta)
{
	mCachedMoveDelta = MoveDelta;
}

void ATTankPawn::AddTankRotationInput(float RotationDelta)
{
	mCachedRotationDelta = RotationDelta;
}

void ATTankPawn::AddTankTurretRotationInput(FRotator RotationDelta)
{
	mCachedTurretRotationDelta = RotationDelta;
}
