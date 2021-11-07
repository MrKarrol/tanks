#include "TPlayerPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Tanks/Guns/TGun.h"
#include "Tanks/Tanks.h"

ATPlayerPawn::ATPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);

	BodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BodyMeshComponent");
	BodyMeshComponent->SetupAttachment(BoxComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TurretMeshComponent");
	TurretMeshComponent->SetupAttachment(BodyMeshComponent);

	GunPivotLocation = CreateDefaultSubobject<UArrowComponent>("GunPivotLocation");
	GunPivotLocation->SetupAttachment(TurretMeshComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(BodyMeshComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	SetGun(DefaultGunClass);
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

void ATPlayerPawn::PerformRightTurretRotation()
{
	if (auto controller = Cast<APlayerController>(GetController()))
	{
		FHitResult result;
		controller->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, result);
		if (result.Location.IsNearlyZero())
			return;
		FRotator new_turret_direction = UKismetMathLibrary::FindLookAtRotation(TurretMeshComponent->GetComponentLocation(), result.Location);
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("%s"), *result.Location.ToString()));
		new_turret_direction.Roll = TurretMeshComponent->GetComponentRotation().Roll;
		new_turret_direction.Pitch = TurretMeshComponent->GetComponentRotation().Pitch;
		TurretMeshComponent->SetWorldRotation(new_turret_direction);
	}
	
}

void ATPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformMovement(DeltaTime);
	PerformRightTurretRotation();
}

void ATPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPlayerPawn::MoveRight);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ATPlayerPawn::StartFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ATPlayerPawn::StopFire);
	PlayerInputComponent->BindAction("AlternateFire", EInputEvent::IE_Pressed, this, &ATPlayerPawn::AlternateFire);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &ATPlayerPawn::Reload);
}

void ATPlayerPawn::MoveForward(float AxisValue)
{
	mMoveForwardInput = AxisValue;
}

void ATPlayerPawn::MoveRight(float AxisValue)
{
	mMoveRightInput = AxisValue;
}

void ATPlayerPawn::StartFire()
{
	if (mGun)
	{
		mGun->StartFire();
	}
	else
	{
		UE_LOG(LogT, Error, TEXT("Gun is null"));
	}
}

void ATPlayerPawn::StopFire()
{
	if (mGun)
	{
		mGun->StopFire();
	}
	else
	{
		UE_LOG(LogT, Error, TEXT("Gun is null"));
	}
}

void ATPlayerPawn::AlternateFire()
{
	if (mGun)
	{
		mGun->AlternateFire();
	}
	else
	{
		UE_LOG(LogT, Error, TEXT("Gun is null"));
	}
}

void ATPlayerPawn::Reload()
{
	if (mGun)
	{
		mGun->Reload();
	}
	else
	{
		UE_LOG(LogT, Error, TEXT("Gun is null"));
	}
}

void ATPlayerPawn::SetGun(TSubclassOf<ATGun> GunClass)
{
	if (GunClass)
	{
		if (mGun)
			mGun->Destroy();

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		mGun = GetWorld()->SpawnActor<ATGun>(GunClass, GunPivotLocation->GetComponentLocation(), GunPivotLocation->GetComponentRotation(), spawnParams);
		mGun->AttachToComponent(GunPivotLocation, FAttachmentTransformRules::SnapToTargetIncludingScale, "Gun");
	}
}
