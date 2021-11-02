#include "TPlayerPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

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

	// move
	mCurrentMoveSpeed = FMath::FInterpConstantTo(mCurrentMoveSpeed, MoveSpeed * FMath::Abs(mMoveForwardInput), DeltaTime, MoveAcceleration);
	const FVector position_delta = GetActorForwardVector() * mCurrentMoveSpeed * DeltaTime * FMath::Sign(mMoveForwardInput);
	SetActorLocation(GetActorLocation() + position_delta);
	
	// rotation
	mCurrentRotationSpeed = FMath::FInterpConstantTo(mCurrentRotationSpeed, RotationSpeed * FMath::Abs(mMoveRightInput), DeltaTime, RotationAcceleration);
	const FRotator rotation_delta = FRotator(0.f, mCurrentRotationSpeed * DeltaTime * FMath::Sign(mMoveRightInput), 0.f);
	SetActorRotation(GetActorRotation() + rotation_delta);

	// turret rotation
	if (auto controller = Cast<APlayerController>(GetController()))
	{
		FVector mouse_location, mouse_direction;
		controller->DeprojectMousePositionToWorld(mouse_location, mouse_direction);

		FVector correct_mouse_location = mouse_location;

		FVector correct_mouse_direction = mouse_direction;
		correct_mouse_direction.Z = 0;
		correct_mouse_direction.Normalize();

		correct_mouse_location = TurretMeshComponent->GetComponentLocation() + correct_mouse_direction * 100.f;

		//DrawDebugPoint(GetWorld(), mouse_location, 10.f, FColor::Red, false);
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("%s"), *mouse_location.ToString()));

		FRotator new_turret_direction = UKismetMathLibrary::FindLookAtRotation(TurretMeshComponent->GetComponentLocation(), correct_mouse_location);
		TurretMeshComponent->SetWorldRotation(new_turret_direction);
	}
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
