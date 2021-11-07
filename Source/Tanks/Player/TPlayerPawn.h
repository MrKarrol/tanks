#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TPlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ATGun;
class UArrowComponent;
class UBoxComponent;

UCLASS()
class TANKS_API ATPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	ATPlayerPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void StartFire();
	void StopFire();

	void AlternateFire();

	void Reload();

	void SetGun(TSubclassOf<ATGun> GunClass);

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
		TSubclassOf<ATGun> DefaultGunClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BoxComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* GunPivotLocation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite,	Category = "Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 512.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveAcceleration = 1024.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationAcceleration = 64.f;

protected:
	virtual void BeginPlay() override;

	void PerformMovement(float DeltaTime);
	void PerformTurretRotation();
	void PerformRightTurretRotation();

private:
	UPROPERTY()
		ATGun* mGun = nullptr;

	float mCurrentMoveSpeed = 0.f;
	float mCurrentRotationSpeed = 0.f;

	float mMoveForwardInput;
	float mMoveRightInput;

};
