#include "TPlayerPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


ATPlayerPawn::ATPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BodyMeshComponent");
	SetRootComponent(BodyMeshComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TurretMeshComponent");
	TurretMeshComponent->SetupAttachment(BodyMeshComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(BodyMeshComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATPlayerPawn::PerformMovement(float DeltaTime)
{
	//FVector currentLocation = GetActorLocation();
	//FVector forwardVector = GetActorForwardVector();
	//FVector rightVector = GetActorRightVector();
	//FVector movePosition = currentLocation
	//	+ forwardVector * MoveSpeed * mMoveForwardInput * DeltaTime // forward - backward
	//	+ rightVector * MoveSpeed * mMoveRightInput * DeltaTime; // right - left
	//SetActorLocation(movePosition, true);

	mCurrentMoveSpeed = FMath::FInterpConstantTo(mCurrentMoveSpeed, MoveSpeed * FMath::Abs(mMoveForwardInput), DeltaTime, MoveAcceleration);
	const FVector position_delta = GetActorForwardVector() * mCurrentMoveSpeed * DeltaTime * FMath::Sign(mMoveForwardInput);
	SetActorLocation(GetActorLocation() + position_delta);
	
	mCurrentRotationSpeed = FMath::FInterpConstantTo(mCurrentRotationSpeed, RotationSpeed * FMath::Abs(mMoveRightInput), DeltaTime, RotationAcceleration);
	const FRotator rotation_delta = FRotator(0.f, mCurrentRotationSpeed * DeltaTime * FMath::Sign(mMoveRightInput), 0.f);
	SetActorRotation(GetActorRotation() + rotation_delta);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *CameraComponent->GetComponentRotation().ToString()));
}

void ATPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformMovement(DeltaTime);
}

void ATPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPlayerPawn::MoveRight);
}

void ATPlayerPawn::MoveForward(float AxisValue)
{
	mMoveForwardInput = AxisValue;
}

void ATPlayerPawn::MoveRight(float AxisValue)
{
	mMoveRightInput = AxisValue;
}
