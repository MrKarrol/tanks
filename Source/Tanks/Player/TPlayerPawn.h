#pragma once

#include "CoreMinimal.h"
#include "Tanks/Core/TTankPawn.h"
#include "Tanks/Interfaces/IScoreTaker.h"
#include "TPlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;
class ATGun;


UCLASS()
class TANKS_API ATPlayerPawn : public ATTankPawn, public IIScoreTaker
{
	GENERATED_BODY()

public:
	ATPlayerPawn();
	void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetGun(TSubclassOf<ATGun> GunClass) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveTurretUp(float AxisValue);
	void MoveTurretRight(float AxisValue);

	//= Begin IScoreTaker interface
	void TakeScore(float Score) override;
	//= End IScoreTaker interface

protected:
	void BeginPlay() override;
	void ChangeGun(TSubclassOf<ATGun> GunClass) override;
	void CalculateTopDownTurretRotation();
	void CalculateThirdViewTurretRotation(float DeltaTime);
	void OnShot(ATGun*);
	void OnDie() override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnPawnDied();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite,	Category = "Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmThirdViewComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent* CameraThirdViewComponent;

private:
	void DefineCameraView(ATGun*);

private:
	void ShowScore() const;

private:
	float TotalScore = 0.f;
	bool bInThirdPersonView = false;

protected:
	float mCurrentTurretUpSpeed = 0.f;
	float mCurrentTurretRotationSpeed = 0.f;

private:
	float mCachedTurretUpDelta;
	float mCachedTurretRotationDelta;

};
