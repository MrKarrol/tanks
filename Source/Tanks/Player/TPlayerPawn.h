#pragma once

#include "CoreMinimal.h"
#include "Tanks/Core/TPawn.h"
#include "TPlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;

UCLASS()
class TANKS_API ATPlayerPawn : public ATPawn
{
	GENERATED_BODY()

public:
	ATPlayerPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

protected:
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
	USceneComponent* GetGunPivotAttach() const override;

private:
	float mCurrentMoveSpeed = 0.f;
	float mCurrentRotationSpeed = 0.f;

	float mMoveForwardInput;
	float mMoveRightInput;

};
