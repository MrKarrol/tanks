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
}

void ATPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATPlayerPawn::ChangeGun(TSubclassOf<ATGun> GunClass)
{
	Super::ChangeGun(GunClass);

	mGun->OnGetScoreDelegate.BindUObject(this, &ATPlayerPawn::TakeScore);
}

void ATPlayerPawn::CalculateTurretRotation()
{
	if (auto controller = Cast<APlayerController>(GetController()))
	{
		FHitResult result;
		controller->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, result);
		if (result.Location.IsNearlyZero())
			return;
		FRotator new_turret_direction = UKismetMathLibrary::FindLookAtRotation(GetGunPivotAttach()->GetComponentLocation(), result.Location);
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("%s"), *result.Location.ToString()));
		new_turret_direction.Roll = GetGunPivotAttach()->GetComponentRotation().Roll;
		new_turret_direction.Pitch = GetGunPivotAttach()->GetComponentRotation().Pitch;
	
		AddTankTurretRotationInput(new_turret_direction);
	}
}

void ATPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateTurretRotation();
	ShowScore();
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

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ATPlayerPawn::StartFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ATPlayerPawn::StopFire);
	PlayerInputComponent->BindAction("AlternateFire", EInputEvent::IE_Pressed, this, &ATPlayerPawn::AlternateFire);
	PlayerInputComponent->BindAction("SwapGuns", EInputEvent::IE_Pressed, this, &ATPlayerPawn::SwapGuns);
}

void ATPlayerPawn::SetGun(TSubclassOf<ATGun> GunClass)
{
	if (mGun)
		mGun->OnShotDelegate.RemoveAll(this);

	Super::SetGun(GunClass);

	if (mGun)
		mGun->OnShotDelegate.AddUObject(this, &ATPlayerPawn::OnShot);
}

void ATPlayerPawn::MoveForward(float AxisValue)
{
	AddTankMovementInput(AxisValue);
}

void ATPlayerPawn::MoveRight(float AxisValue)
{
	AddTankRotationInput(AxisValue);
}

void ATPlayerPawn::TakeScore(float Score)
{
	TotalScore += Score;
}

void ATPlayerPawn::OnShot(ATGun*)
{
	auto controller = Cast<APlayerController>(GetController());
	if (mGun && mGun->ShotCameraShake && controller)
	{
		controller->ClientStartCameraShake(mGun->ShotCameraShake);
	}
}