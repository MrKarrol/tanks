#include "TPlayerPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Tanks/Tanks.h"
#include "Tanks/Guns/TGun.h"


ATPlayerPawn::ATPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	GunPivotLocation = CreateDefaultSubobject<UArrowComponent>("GunPivotLocation");
	GunPivotLocation->SetupAttachment(BodyMeshComponent);

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

void ATPlayerPawn::PerformMovement(float DeltaTime)
{
	// move
	mCurrentMoveSpeed = FMath::FInterpConstantTo(mCurrentMoveSpeed, MoveSpeed * FMath::Abs(mMoveForwardInput), DeltaTime, MoveAcceleration);
	const FVector position_delta = GetActorForwardVector() * mCurrentMoveSpeed * DeltaTime * FMath::Sign(mMoveForwardInput);
	SetActorLocation(GetActorLocation() + position_delta);
	
	// rotation
	mCurrentRotationSpeed = FMath::FInterpConstantTo(mCurrentRotationSpeed, RotationSpeed * FMath::Abs(mMoveRightInput), DeltaTime, RotationAcceleration);
	const FRotator rotation_delta = FRotator(0.f, mCurrentRotationSpeed * DeltaTime * FMath::Sign(mMoveRightInput), 0.f);
	SetActorRotation(GetActorRotation() + rotation_delta);
}

void ATPlayerPawn::PerformTurretRotation()
{
	if (auto controller = Cast<APlayerController>(GetController()))
	{
		FHitResult result;
		controller->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, result);
		if (result.Location.IsNearlyZero())
			return;
		FRotator new_turret_direction = UKismetMathLibrary::FindLookAtRotation(GunPivotLocation->GetComponentLocation(), result.Location);
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("%s"), *result.Location.ToString()));
		new_turret_direction.Roll = GunPivotLocation->GetComponentRotation().Roll;
		new_turret_direction.Pitch = GunPivotLocation->GetComponentRotation().Pitch;
		GunPivotLocation->SetWorldRotation(new_turret_direction);
	}
	
}

USceneComponent* ATPlayerPawn::GetGunPivotAttach() const
{
	return GunPivotLocation;
}

void ATPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformMovement(DeltaTime);
	PerformTurretRotation();

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

void ATPlayerPawn::MoveForward(float AxisValue)
{
	mMoveForwardInput = AxisValue;
}

void ATPlayerPawn::MoveRight(float AxisValue)
{
	mMoveRightInput = AxisValue;
}

void ATPlayerPawn::TakeScore(float Score)
{
	TotalScore += Score;
}