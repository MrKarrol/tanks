#include "TPlayerPawn.h"

#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Tanks/Components/THealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Tanks/Components/TInventoryComponent.h"
#include "Tanks/Components/TEquipInventoryComponent.h"
#include "Tanks/Components/TInventoryManagerComponent.h"
#include "Tanks/Guns/TGun.h"


ATPlayerPawn::ATPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(BodyMeshComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmThirdViewComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmThirdViewComponent");
	SpringArmThirdViewComponent->SetupAttachment(BodyMeshComponent);

	CameraThirdViewComponent = CreateDefaultSubobject<UCameraComponent>("CameraThirdViewComponent");
	CameraThirdViewComponent->SetupAttachment(SpringArmThirdViewComponent);
	CameraThirdViewComponent->Deactivate();

	InventoryComponent = CreateDefaultSubobject<UTInventoryComponent>("InventoryComponent");
	EquipmentInventoryComponent = 
		CreateDefaultSubobject<UTEquipInventoryComponent>("EquipInventory");
	InventoryManagerComponent = CreateDefaultSubobject<UTInventoryManagerComponent>("InventoryManagerComponent");
}

void ATPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (const auto gun = GetCurrentGun())
	{
		gun->OnShotDelegate.AddUObject(this, &ATPlayerPawn::OnShot);
		gun->OnGetScoreDelegate.BindUObject(this, &ATPlayerPawn::TakeScore);
	}

	InventoryManagerComponent->Init(InventoryComponent);
	InventoryManagerComponent->InitEquipment(EquipmentInventoryComponent);
}

void ATPlayerPawn::SwapGuns()
{
	Super::SwapGuns();

	if (const auto gun = GetCurrentGun())
		DefineCameraView(gun);
}

void ATPlayerPawn::AddTurretHelper(ATEnemyTurret* helper)
{
	if (mTurretHelpers.Num() >= MaxTurretHelpersCount)
		return;

	mTurretHelpers.Emplace(helper);
}

bool ATPlayerPawn::CanSpawnTurretHelper() const
{
	return mTurretHelpers.Num() < MaxTurretHelpersCount;
}

size_t ATPlayerPawn::TurretHelpersNumber() const
{
	return mTurretHelpers.Num();
}

void ATPlayerPawn::CalculateTopDownTurretRotation()
{
	if (const auto controller = Cast<APlayerController>(GetController()))
	{
		FHitResult result;
		controller->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, result);
		if (result.Location.IsNearlyZero())
			return;
		FRotator new_turret_direction = UKismetMathLibrary::FindLookAtRotation(GetGunPivotAttach()->GetComponentLocation(), result.Location);
		new_turret_direction.Roll = GetGunPivotAttach()->GetComponentRotation().Roll;
		new_turret_direction.Pitch = GetGunPivotAttach()->GetComponentRotation().Pitch;
	
		AddTankTurretRotationInput(new_turret_direction);
	}
}

void ATPlayerPawn::CalculateThirdViewTurretRotation(float DeltaTime)
{
	FRotator summary_rotation_delta;
	if (FMath::IsNearlyZero(mCachedTurretUpDelta))
		mCurrentTurretUpSpeed = 0.f;
	else if (const auto gun = GetCurrentGun())
	{
		mCurrentTurretUpSpeed = FMath::FInterpConstantTo(mCurrentTurretUpSpeed, gun->TurretRotationSpeed * FMath::Abs(mCachedTurretUpDelta), DeltaTime, gun->TurretRotationAcceleration);
		const FRotator rotation_delta = FRotator(mCurrentTurretUpSpeed * DeltaTime * FMath::Sign(mCachedTurretUpDelta), 0.f, 0.f);
		summary_rotation_delta += rotation_delta;
		mCachedTurretUpDelta = 0.f;
	}

	if (FMath::IsNearlyZero(mCachedTurretRotationDelta))
		mCurrentTurretRotationSpeed = 0.f;
	else if (const auto gun = GetCurrentGun())
	{
		mCurrentTurretRotationSpeed = FMath::FInterpConstantTo(mCurrentTurretRotationSpeed, gun->TurretRotationSpeed * FMath::Abs(mCachedTurretRotationDelta), DeltaTime, gun->TurretRotationAcceleration);
		const FRotator rotation_delta = FRotator(0.f, mCurrentTurretRotationSpeed * DeltaTime * FMath::Sign(mCachedTurretRotationDelta), 0.f);
		summary_rotation_delta += rotation_delta;

		mCachedTurretRotationDelta = 0.f;
	}

	AddTankTurretRotationInput(GetGunPivotAttach()->GetComponentRotation() + summary_rotation_delta);
}

void ATPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bInThirdPersonView)
		CalculateTopDownTurretRotation();
	else
		CalculateThirdViewTurretRotation(DeltaTime);
}

void ATPlayerPawn::DefineCameraView(ATGun* gun)
{
	if (gun->NeedThirdView)
	{
		CameraComponent->Deactivate();
		CameraThirdViewComponent->Activate();
		bInThirdPersonView = true;
	}
	else
	{
		CameraThirdViewComponent->Deactivate();
		CameraComponent->Activate();
		bInThirdPersonView = false;
	}
}

void ATPlayerPawn::SetGun(TSubclassOf<ATGun> GunClass)
{
	Super::SetGun(GunClass);

	if (auto gun = GetCurrentGun())
	{
		gun->OnShotDelegate.AddUObject(this, &ATPlayerPawn::OnShot);
		gun->OnGetScoreDelegate.BindUObject(this, &ATPlayerPawn::TakeScore);
	}
}

void ATPlayerPawn::MoveForward(float AxisValue)
{
	AddTankMovementInput(AxisValue);
}

void ATPlayerPawn::MoveRight(float AxisValue)
{
	AddTankRotationInput(AxisValue);
}

void ATPlayerPawn::MoveTurretUp(float AxisValue)
{
	mCachedTurretUpDelta = FMath::Clamp(AxisValue, -1.f, 1.f);
}

void ATPlayerPawn::MoveTurretRight(float AxisValue)
{
	mCachedTurretRotationDelta = FMath::Clamp(AxisValue, -1.f, 1.f);
}

void ATPlayerPawn::TakeScore(float InScore)
{
	TotalScore += InScore;
	if (OnChangeScoreDelegate.IsBound())
		OnChangeScoreDelegate.Execute(FMath::CeilToInt(TotalScore));
}

void ATPlayerPawn::Heal(float HealthGain)
{
	HealthComponent->SetHealth(HealthComponent->GetHealth() + HealthGain);
}

void ATPlayerPawn::OnShot(ATGun*) const
{
	const auto controller = Cast<APlayerController>(GetController());
	const auto gun = GetCurrentGun();
	if (gun && gun->ShotCameraShake && controller)
	{
		controller->ClientStartCameraShake(gun->ShotCameraShake);
	}
}

void ATPlayerPawn::OnDie()
{
	OnPawnDied();
}
