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
	mCachedMoveDelta = FMath::Clamp(MoveDelta, -1.f, 1.f);
}

void ATTankPawn::AddTankRotationInput(float RotationDelta)
{
	mCachedRotationDelta = FMath::Clamp(RotationDelta, -1.f, 1.f);
}

void ATTankPawn::AddTankTurretRotationInput(FRotator RotationDelta)
{
	if (!RotationDelta.IsNearlyZero())
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("%s"), *RotationDelta.ToString()));
	mCachedTurretRotationDelta = RotationDelta;
}
