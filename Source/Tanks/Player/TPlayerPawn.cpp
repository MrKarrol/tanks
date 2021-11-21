#include "TPlayerPawn.h"

#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Tanks/Tanks.h"
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
}

void ATPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATPlayerPawn::ChangeGun(TSubclassOf<ATGun> GunClass)
{
	Super::ChangeGun(GunClass);

	DefineCameraView(mGun);
}

void ATPlayerPawn::CalculateTopDownTurretRotation()
{
	if (auto controller = Cast<APlayerController>(GetController()))
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
	else
	{
		mCurrentTurretUpSpeed = FMath::FInterpConstantTo(mCurrentTurretUpSpeed, mGun->TurretRotationSpeed * FMath::Abs(mCachedTurretUpDelta), DeltaTime, mGun->TurretRotationAcceleration);
		const FRotator rotation_delta = FRotator(mCurrentTurretUpSpeed * DeltaTime * FMath::Sign(mCachedTurretUpDelta), 0.f, 0.f);
		summary_rotation_delta += rotation_delta;
		mCachedTurretUpDelta = 0.f;
	}

	if (FMath::IsNearlyZero(mCachedTurretRotationDelta))
		mCurrentTurretRotationSpeed = 0.f;
	else
	{
		mCurrentTurretRotationSpeed = FMath::FInterpConstantTo(mCurrentTurretRotationSpeed, mGun->TurretRotationSpeed * FMath::Abs(mCachedTurretRotationDelta), DeltaTime, mGun->TurretRotationAcceleration);
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
	
	ShowScore();
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

void ATPlayerPawn::ShowScore() const
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Orange, FString::Printf(TEXT("Score: %f"), TotalScore));
}

void ATPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPlayerPawn::MoveRight);

	PlayerInputComponent->BindAxis("MoveTurretUp", this, &ATPlayerPawn::MoveTurretUp);
	PlayerInputComponent->BindAxis("MoveTurretRight", this, &ATPlayerPawn::MoveTurretRight);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ATPlayerPawn::StartFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ATPlayerPawn::StopFire);
	PlayerInputComponent->BindAction("AlternateFire", EInputEvent::IE_Pressed, this, &ATPlayerPawn::AlternateFire);
	PlayerInputComponent->BindAction("SwapGuns", EInputEvent::IE_Pressed, this, &ATPlayerPawn::SwapGuns);
}

void ATPlayerPawn::SetGun(TSubclassOf<ATGun> GunClass)
{
	if (mGun)
	{
		mGun->OnShotDelegate.RemoveAll(this);
		mGun->OnGetScoreDelegate.Unbind();
	}

	Super::SetGun(GunClass);

	if (mGun)
	{
		mGun->OnShotDelegate.AddUObject(this, &ATPlayerPawn::OnShot);
		mGun->OnGetScoreDelegate.BindUObject(this, &ATPlayerPawn::TakeScore);
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
}

void ATPlayerPawn::OnShot(ATGun*)
{
	auto controller = Cast<APlayerController>(GetController());
	if (mGun && mGun->ShotCameraShake && controller)
	{
		controller->ClientStartCameraShake(mGun->ShotCameraShake);
	}
}