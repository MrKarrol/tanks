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
	
	GunClassFirst = DefaultGunClassFirst;
	GunClassSecond = DefaultGunClassSecond;

	ChangeGun(GunClassFirst);
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



void ATPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("%s"), *GunPivotLocation->GetComponentLocation().ToString()));

	PerformMovement(DeltaTime);
	PerformTurretRotation();
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

void ATPlayerPawn::SwapGuns()
{
	switch (mCurrentGun)
	{
	case 0:
		if (GunClassSecond)
		{
			ChangeGun(GunClassSecond);
			mCurrentGun = 1;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("You now powered with second gun"));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("You haven't second gun"));
		break;
	case 1:
		if (GunClassSecond)
		{
			ChangeGun(GunClassFirst);
			mCurrentGun = 0;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("You now powered with first gun"));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("You haven't first gun"));
		break;
	}
}

void ATPlayerPawn::SetGun(TSubclassOf<ATGun> GunClass)
{
	switch (mCurrentGun)
	{
	case 0:
		if (!GunClassSecond)
		{
			GunClassSecond = GunClass;
			SwapGuns();
			return;
		}
		GunClassFirst = GunClass;
		ChangeGun(GunClassFirst);
		break;
	case 1:
		if (!GunClassFirst)
		{
			GunClassFirst = GunClass;
			SwapGuns();
			return;
		}
		GunClassSecond = GunClass;
		ChangeGun(GunClassSecond);
		break;
	}
}

void ATPlayerPawn::ChangeGun(TSubclassOf<ATGun> GunClass)
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
